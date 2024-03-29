import db from '../database.js';
import account from './account_model.js';
import transaction from './transaction_model.js';

const BLOCKCHAIN_API_URL = 'https://blockchain.info';

const bitcoin = {
  getBitcoinAccount: function (idaccount, callback) {
    db.query(
      `SELECT bitcoin_account.*
      FROM account AS bitcoin_account
      WHERE bitcoin_account.account_type = 'bitcoin'
      AND bitcoin_account.idcustomer IN (SELECT debit_account.idcustomer
                                        FROM account AS debit_account
                                        WHERE debit_account.idaccount = ?
                                        AND debit_account.account_type = 'debit');`,
      [idaccount],
      (err, results) => {
        if (err) callback(err, null);
        else callback(null, results[0]);
      }
    );
  },
  eurosToBitcoin: async function (euros) {
    const response = await fetch(`${BLOCKCHAIN_API_URL}/tobtc?currency=EUR&value=${euros}`);
    return response.text();
  },
  getLastPrice: async function () {
    const response = await fetch(`${BLOCKCHAIN_API_URL}/ticker`);
    const json = await response.json();
    return json.EUR.last;
  },
  getBitcoinAccountByDebitAccountId: function (idaccount, callback) {
    bitcoin.getBitcoinAccount(idaccount, async function (_, bitcoinAccount) {
      if (!bitcoinAccount) {
        return callback(new Error('Bitcoin account not found'), null);
      }
      const lastPrice = await bitcoin.getLastPrice();
      return callback(null, {
        bitcoin_balance_eur: (
          Math.round(parseFloat(bitcoinAccount.bitcoin_balance) * lastPrice * 100) / 100
        ).toString(),
        ...bitcoinAccount,
      });
    });
  },
  buy: async function (idaccount, euros, callback) {
    account.getById(idaccount, async function (_, results) {
      if (results.length === 0) {
        return callback(new Error('Account not found'), null);
      }

      const debitAccount = results[0];
      if (debitAccount.account_type !== 'debit') {
        return callback(new Error('Account must be of type debit'), null);
      }

      bitcoin.getBitcoinAccount(idaccount, async function (_, bitcoinAccount) {
        if (!bitcoinAccount) {
          return callback(new Error('Bitcoin account not found'), null);
        }

        if (parseFloat(debitAccount.balance) < euros) {
          return callback(new Error('Insufficient balance'), null);
        }

        const newBalance = Math.round((parseFloat(debitAccount.balance) - euros) * 100) / 100;

        account.update(debitAccount.idaccount, {
          account_type: debitAccount.account_type,
          account_name: debitAccount.account_name,
          balance: newBalance,
          credit_limit: debitAccount.credit_limit,
          bitcoin_balance: debitAccount.bitcoin_balance,
          idcustomer: debitAccount.idcustomer,
        });

        const bitcoins = parseFloat(await bitcoin.eurosToBitcoin(euros));
        const newBitcoinBalance = parseFloat(bitcoinAccount.bitcoin_balance) + bitcoins;

        account.update(bitcoinAccount.idaccount, {
          account_type: bitcoinAccount.account_type,
          account_name: bitcoinAccount.account_name,
          balance: bitcoinAccount.balance,
          credit_limit: bitcoinAccount.credit_limit,
          bitcoin_balance: newBitcoinBalance,
          idcustomer: bitcoinAccount.idcustomer,
        });

        transaction.add({
          amount: -euros,
          transaction_type: 'withdraw',
          transaction_description: 'Bitcoin osto',
          idaccount: debitAccount.idaccount,
        });

        transaction.addBitcoin({
          bitcoin_amount: bitcoins,
          transaction_type: 'deposit',
          transaction_description: 'Bitcoin osto',
          idaccount: bitcoinAccount.idaccount,
        });

        return callback(null, {
          new_balance: newBalance.toString(),
          new_bitcoin_balance: newBitcoinBalance.toFixed(8),
        });
      });
    });
  },
  getLastFiveTransactions: function (idaccount, callback) {
    bitcoin.getBitcoinAccount(idaccount, function (_, bitcoinAccount) {
      const query = `SELECT * FROM transaction WHERE idaccount = ? AND bitcoin_amount IS NOT NULL ORDER BY transaction_date DESC LIMIT 5`;
      db.query(query, [bitcoinAccount.idaccount], callback);
    });
  },
};

export default bitcoin;
