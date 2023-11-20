import db from '../database.js';

const account = {
  getAll: function (callback) {
    return db.query('select * from account', callback);
  },
  getById: function (id, callback) {
    return db.query('select * from account where idaccount=?', [id], callback);
  },
  add: function (account, callback) {
    return db.query(
      'insert into account (account_type, account_name, balance, credit_limit, idcustomer) values(?,?,?,?,?)',
      [
        account.account_type,
        account.account_name,
        account.balance,
        account.credit_limit,
        account.idcustomer,
      ],
      callback
    );
  },
  delete: function (id, callback) {
    return db.query('delete from account where idaccount=?', [id], callback);
  },
  update: function (id, account, callback) {
    return db.query(
      'update account set account_type=?, account_name=?, balance=?, credit_limit=?, bitcoin_balance=?, idcustomer=? where idaccount=?',
      [
        account.account_type,
        account.account_name,
        account.balance,
        account.credit_limit,
        account.bitcoin_balance,
        account.idcustomer,
        id,
      ],
      callback
    );
  },
};

export default account;
