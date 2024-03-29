import db from '../database.js';
import bcrypt from 'bcryptjs';

const saltRounds = 10;

const card = {
  getAll: function (callback) {
    return db.query('select * from card', callback);
  },
  getById: function (id, callback) {
    return db.query('select * from card where idcard=?', [id], callback);
  },
  add: function (card, callback) {
    bcrypt.hash(card.pin, saltRounds, function (_, hash) {
      return db.query(
        'insert into card (pin, card_type, idcustomer) values(?,?,?)',
        [hash, card.card_type, card.idcustomer],
        callback
      );
    });
  },
  delete: function (id, callback) {
    return db.query('delete from card where idcard=?', [id], callback);
  },
  update: function (id, card, callback) {
    bcrypt.hash(card.pin, saltRounds, function (_, hash) {
      return db.query(
        'update card set pin=?, card_type=?, idcustomer=? where idcard=?',
        [hash, card.card_type, card.idcustomer, id],
        callback
      );
    });
  },
};

export default card;
