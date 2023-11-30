import db from '../database.js';
import account from './account_model.js';
import transaction from './transaction_model.js';

const BLOCKCHAIN_API_URL = 'https://blockchain.info';

async function eurosToBitcoin(euros) {
  const response = await fetch(`${BLOCKCHAIN_API_URL}/tobtc?currency=EUR&value=${euros}`);
  return response.json();
}

function getBitcoinAccount(idaccount, callback) {
  db.query(
    `SELECT bitcoin_account.*
    FROM account AS debit_account
    INNER JOIN account AS bitcoin_account ON debit_account.idcustomer = bitcoin_account.idcustomer
    INNER JOIN account_access ON bitcoin_account.idaccount = account_access.idaccount
    INNER JOIN card ON account_access.idcard = card.idcard
    WHERE debit_account.idaccount = ?
        AND debit_account.account_type = 'debit'
        AND bitcoin_account.account_type = 'bitcoin'
        AND debit_account.idcustomer = bitcoin_account.idcustomer`,
    [idaccount],
    (err, results) => {
      if (err) callback(err, null);
      else callback(null, results[0]);
    }
  );
}

const bitcoin = {
  getLastPrice: async function () {
    const response = await fetch(`${BLOCKCHAIN_API_URL}/ticker`);
    const json = await response.json();
    return json.EUR.last;
  },
  getBalanceByAccountId: function (idaccount, callback) {
    getBitcoinAccount(idaccount, function (_, bitcoinAccount) {
      if (!bitcoinAccount) {
        return callback(new Error('Bitcoin account not found'), null);
      }
      return callback(null, bitcoinAccount.bitcoin_balance);
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

      getBitcoinAccount(idaccount, async function (_, bitcoinAccount) {
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

        const bitcoins = await eurosToBitcoin(euros);
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
          transaction_description: 'Bitcoin purchase',
          idaccount: debitAccount.idaccount,
        });

        transaction.addBitcoin({
          bitcoin_amount: bitcoins,
          transaction_type: 'deposit',
          transaction_description: 'Bitcoin purchase',
          idaccount: bitcoinAccount.idaccount,
        });

        return callback(null, {
          euros: euros,
          balance: newBalance,
          bitcoin_balance: newBitcoinBalance,
        });
      });
    });
  },
};

export default bitcoin;
