import { Router } from 'express';
import bitcoin from '../models/bitcoin_model.js';

export const router = Router();

router.get('/account/:id', async function (request, response) {
  bitcoin.getBitcoinAccountByDebitAccountId(request.params.id, function (error, results) {
    if (error) {
      return response.status(400).json({ error: error.message });
    }
    return response.json(results);
  });
});

router.post('/buy', async function (request, response) {
  const { idaccount, euros } = request.body;
  console.log('euros =', euros);
  if (euros <= 0 || euros > Number.MAX_SAFE_INTEGER || isNaN(euros)) {
    return response.status(400).json({ error: 'Amount must be positive' });
  }

  bitcoin.buy(idaccount, euros, function (error, results) {
    if (error) {
      return response.status(400).json({ error: error.message });
    }

    return response.json(results);
  });
});

router.get('/lastprice', async function (_, response) {
  return response.json(await bitcoin.getLastPrice());
});

router.get('/tobtc', async function (request, response) {
  const { euros } = request.query;

  if (euros <= 0) {
    return response.status(400).json({ error: 'Amount must be positive' });
  }

  const btc = await bitcoin.eurosToBitcoin(euros);

  return response.json({
    btc,
  });
});

router.get('/account/:idaccount/transactions', function (request, response) {
  bitcoin.getLastFiveTransactions(parseInt(request.params.idaccount), function (error, results) {
    if (error) {
      return response.status(400).json({ error: error.message });
    }
    return response.json({
      transactions: results,
    });
  });
});
