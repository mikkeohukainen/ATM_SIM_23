import db from '../database.js';

const transactions = {
  getFiveById: function (idaccount, startIndex, callback) {
    const query =
      'SELECT * FROM transaction WHERE idaccount = ? ORDER BY transaction_date DESC LIMIT 5 OFFSET ?';
    db.query(query, [idaccount, startIndex], callback);
  },
};

export default transactions;
