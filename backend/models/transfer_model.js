import db from '../database.js';

function validateAmount(amount) {
  const amountStr = amount.toString();
  const decimalPointIndex = amountStr.indexOf('.');

  if (decimalPointIndex !== -1) {
    const numberOfDecimals = amountStr.length - decimalPointIndex - 1;
    if (numberOfDecimals > 2) {
      return 'Incorrect amount, max of 2 decimals allowed';
    }
  }
  return null; // Amount is valid
}

const transfer = {
  getReceiverInfo: function (receivingAccountId, callback) {
    // Get receivers info (name and account_type)
    db.query(
      'SELECT c.fname, c.lname, a.account_type ' +
        'FROM account a ' +
        'JOIN customer c ON a.idcustomer = c.idcustomer ' +
        'WHERE a.idaccount = ?',
      [receivingAccountId],
      function (err, results) {
        if (err) {
          callback(err, null);
        } else {
          if (results.length > 0) {
            const receiver = results[0];
            const receiverName = receiver.fname + ' ' + receiver.lname;
            const accountType = receiver.account_type;
            callback(null, { name: receiverName, type: accountType });
          } else {
            callback(new Error('Receiver account not found'), null);
          }
        }
      }
    );
  },
  bankTransfer: function (sendingAccountId, receivingAccountId, amount, callback) {
    if (amount <= 0) {
      return callback(new Error('Amount must be positive'), null);
    }
    const validationError = validateAmount(amount);
    if (validationError) {
      return callback(new Error(validationError));
    }

    // Get a connection from the pool
    db.getConnection(function (err, connection) {
      if (err) {
        console.error('Database connection error:', err);
        return callback(new Error('Error connecting to the database'));
      }

      // Begin a new transaction
      connection.beginTransaction(function (err) {
        if (err) {
          connection.release();
          return callback(err);
        }
        // First query: Get receivers info (name and account_type). Check if receiver account is valid.
        transfer.getReceiverInfo(receivingAccountId, function (err, receiverInfo) {
          if (err) {
            return callback(err);
          }

          if (!receiverInfo) {
            return callback(new Error('Receiver account not found'));
          }

          if (receiverInfo.type === 'bitcoin') {
            return callback(new Error('Account must be type Debit or Credit'));
          }

          const receiverName = receiverInfo.name;

          // Second query:  Check sender's account balance and credit limit, get sender's name
          connection.query(
            'SELECT a.balance, a.credit_limit, a.account_type, c.fname, c.lname ' +
              'FROM account a ' +
              'JOIN customer c ON a.idcustomer = c.idcustomer ' +
              'WHERE a.idaccount = ?',
            [sendingAccountId],
            function (err, results) {
              if (err) {
                return connection.rollback(function () {
                  connection.release();
                  callback(err);
                });
              }

              const sendingAccount = results[0];

              if (!sendingAccount) {
                connection.release();
                return callback(new Error('Sending account not found'));
              }
              const senderName = sendingAccount.fname + ' ' + sendingAccount.lname;
              const accountBalance = parseFloat(sendingAccount.balance);
              const creditLimit = parseFloat(sendingAccount.credit_limit);
              const transferAmount = parseFloat(amount);

              if (sendingAccount.account_type === 'debit') {
                if (accountBalance < transferAmount) {
                  connection.release();
                  return callback(new Error('Insufficient funds'));
                }
              } else if (sendingAccount.account_type === 'credit') {
                if (creditLimit + accountBalance < transferAmount) {
                  connection.release();
                  return callback(new Error('Insufficient funds'));
                }
              } else {
                connection.release();
                return callback(new Error('Account must be of type Debit or Credit'));
              }

              // Third query: Update sender account

              connection.query(
                'UPDATE account SET balance = balance - ? WHERE idaccount = ?',
                [amount, sendingAccountId],
                function (err, result) {
                  if (err) {
                    return connection.rollback(function () {
                      connection.release();
                      callback(err);
                    });
                  }

                  if (result.affectedRows === 0) {
                    return connection.rollback(function () {
                      connection.release();
                      callback(new Error('Error updating sender account'));
                    });
                  }

                  // Fourth query: Update receiver account
                  connection.query(
                    'UPDATE account SET balance = balance + ? WHERE idaccount = ?',
                    [amount, receivingAccountId],
                    function (err, result) {
                      if (err) {
                        return connection.rollback(function () {
                          connection.release();
                          callback(err);
                        });
                      }

                      if (result.affectedRows === 0) {
                        return connection.rollback(function () {
                          connection.release();
                          callback(new Error('Error updating receiver account'));
                        });
                      }
                      // Fifth query: Log sender transaction
                      connection.query(
                        'INSERT INTO transaction (idaccount, amount, transaction_type, transaction_date, transaction_description) VALUES (?, ?, "withdraw", NOW(), "Tilisiirto henkilölle ' +
                          receiverName +
                          '")',
                        [sendingAccountId, -amount],
                        function (err, result) {
                          if (err) {
                            return connection.rollback(function () {
                              connection.release();
                              callback(err);
                            });
                          }

                          if (result.affectedRows === 0) {
                            return connection.rollback(function () {
                              connection.release();
                              callback(new Error('Error logging sender transaction'));
                            });
                          }

                          // Sixth query: Log receiver transaction
                          connection.query(
                            'INSERT INTO transaction (idaccount, amount, transaction_type, transaction_date, transaction_description) ' +
                              'VALUES (?, ?, "deposit", NOW(), "Tilisiirto henkilöltä ' +
                              senderName +
                              '")',
                            [receivingAccountId, amount],
                            function (err, result) {
                              if (err) {
                                return connection.rollback(function () {
                                  connection.release();
                                  callback(err);
                                });
                              }

                              if (result.affectedRows === 0) {
                                return connection.rollback(function () {
                                  connection.release();
                                  callback(new Error('Error logging receiver transaction'));
                                });
                              }

                              // Commit the transaction
                              connection.commit(function (err) {
                                if (err) {
                                  return connection.rollback(function () {
                                    connection.release();
                                    callback(err);
                                  });
                                }
                                connection.release();
                                callback(null, result);
                              });
                            }
                          );
                        }
                      );
                    }
                  );
                }
              );
            }
          );
        });
      });
    });
  },
};
export default transfer;
