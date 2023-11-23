import db from '../database.js';

const account_access = {
  getAll: function (callback) {
    return db.query('select * from account_access', callback);
  },
  getByCardId: function (id, callback) {
    return db.query(
      'select group_concat(idaccount) as accounts from account_access where idcard=?',
      [id],
      callback
    );
  },
  add: function (account_access, callback) {
    return db.query(
      'insert into account_access (idcard, idaccount) values(?,?)',
      [account_access.idcard, account_access.idaccount],
      callback
    );
  },
  delete: function (id, callback) {
    return db.query('delete from account_access where idcard_access=?', [id], callback);
  },
  update: function (id, account_access, callback) {
    return db.query(
      'update account_access set idcard=?, idaccount=? where idcard_access=?',
      [account_access.idcard, account_access.idaccount, id],
      callback
    );
  },
};
export default account_access;
