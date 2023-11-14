import db from '../database.js';

const transaction = {
  getAll: function (callback) {
    return db.query('select * from transaction', callback);
  },
  getById: function (id, callback) {
    return db.query('select * from transaction where idtransaction=?', [id], callback);
  },
  add: function (transaction, callback) {
    return db.query(
      'insert into transaction (amount, transaction_type, transaction_date, transaction_description, idaccount) values(?,?,?,?,?)',
      [
        transaction.amount,
        transaction.transaction_type,
        transaction.transaction_date,
        transaction.transaction_description,
        transaction.idaccount,
      ],
      callback
    );
  },
  delete: function (id, callback) {
    return db.query('delete from transaction where idtransaction=?', [id], callback);
  },
  update: function (id, transaction, callback) {
    return db.query(
      'update transaction set amount=?, transaction_type=?, transaction_date=?, transaction_description=?, idaccount=? where idtransaction=?',
      [
        transaction.amount,
        transaction.transaction_type,
        transaction.transaction_date,
        transaction.transaction_description,
        transaction.idaccount,
        id,
      ],
      callback
    );
  },
};

export default transaction;
