import db from '../database.js';

const login = {
  checkPin: function (id, callback) {
    return db.query('SELECT pin FROM card WHERE idcard = ?', [id], callback);
  },
};

export default login;
