import { Router } from 'express';
import bitcoin from '../models/bitcoin_model.js';

export const router = Router();

router.get('/balance/:id', async function (request, response) {
  await bitcoin.getBalanceByAccountId(request.params.id, function (error, results) {
    if (error) {
      return response.status(400).json({ error: error.message });
    }

    return response.json(results);
  });
});

router.post('/buy', async function (request, response) {
  const { idaccount, euros } = request.body;

  if (euros <= 0) {
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
  return response.json({ lastPrice: await bitcoin.getLastPrice() });
});
