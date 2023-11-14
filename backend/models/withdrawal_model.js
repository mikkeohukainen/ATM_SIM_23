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

const withdrawal = {
  creditWithdraw: function (idaccount, amount, callback) {
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

        // First query: check account balance and credit limit
        connection.query(
          'SELECT balance, credit_limit, account_type FROM account WHERE idaccount = ?',
          [idaccount],
          function (err, results) {
            if (err) {
              return connection.rollback(function () {
                connection.release();
                callback(err);
              });
            }

            const account = results[0];

            if (!account) {
              connection.release();
              return callback(new Error('Account not found'));
            }

            if (account.account_type !== 'credit') {
              connection.release();
              return callback(new Error('Account must be of type Credit'));
            }

            const accountBalance = parseFloat(account.balance);
            const withdrawalAmount = parseFloat(amount);
            const creditLimit = parseFloat(account.credit_limit);

            if (accountBalance + creditLimit < withdrawalAmount) {
              return connection.rollback(function () {
                connection.release();
                callback(new Error('Insufficient funds'));
              });
            }

            // Second query: update account balance
            connection.query(
              'UPDATE account SET balance = balance - ? WHERE idaccount = ? AND balance + credit_limit >= ?',
              [amount, idaccount, amount],
              function (err, result) {
                if (err || result.affectedRows === 0) {
                  return connection.rollback(function () {
                    connection.release();
                    callback(err);
                  });
                }

                // Third query: log the transaction
                connection.query(
                  'INSERT INTO transaction (idaccount, amount, transaction_type, transaction_date, transaction_description) VALUES (?, ?, "withdraw", NOW(), "ATM Credit Withdrawal")',
                  [idaccount, -amount],
                  function (err, result) {
                    if (err) {
                      return connection.rollback(function () {
                        connection.release();
                        callback(err);
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
      });
    });
  },

  debitWithdraw: function (idaccount, amount, callback) {
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

        // First query: check account balance and credit limit
        connection.query(
          'SELECT balance, account_type FROM account WHERE idaccount = ?',
          [idaccount],
          function (err, results) {
            if (err) {
              return connection.rollback(function () {
                connection.release();
                callback(err);
              });
            }

            const account = results[0];

            if (!account) {
              connection.release();
              return callback(new Error('Account not found'));
            }

            if (account.account_type !== 'debit') {
              connection.release();
              return callback(new Error('Account must be of type Debit'));
            }

            const accountBalance = parseFloat(account.balance);
            const withdrawalAmount = parseFloat(amount);

            if (accountBalance < withdrawalAmount) {
              return connection.rollback(function () {
                connection.release();
                callback(new Error('Insufficient funds'));
              });
            }

            // Second query: update account balance
            connection.query(
              'UPDATE account SET balance = balance - ? WHERE idaccount = ? AND balance >= ?',
              [amount, idaccount, amount],
              function (err, result) {
                if (err || result.affectedRows === 0) {
                  return connection.rollback(function () {
                    connection.release();
                    callback(err);
                  });
                }

                // Third query: log the transaction
                connection.query(
                  'INSERT INTO transaction (idaccount, amount, transaction_type, transaction_date, transaction_description) VALUES (?, ?, "withdraw", NOW(), "ATM Debit Withdrawal")',
                  [idaccount, -amount],
                  function (err, result) {
                    if (err) {
                      return connection.rollback(function () {
                        connection.release();
                        callback(err);
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
      });
    });
  },
};

export default withdrawal;
