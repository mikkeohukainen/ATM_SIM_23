import { Router } from 'express';
import withdrawal from '../models/withdrawal_model.js';

export const router = Router();

router.post('/credit', function (request, response) {
  const { idaccount, amount } = request.body;
  withdrawal.creditWithdraw(idaccount, amount, function (err, dbResult) {
    if (err) {
      response.status(500).json({ error: err.message });
    } else {
      response.status(200).json({ message: 'Withdrawal successful', transaction: dbResult });
    }
  });
});

router.post('/debit', function (request, response) {
  const { idaccount, amount } = request.body;
  withdrawal.debitWithdraw(idaccount, amount, function (err, dbResult) {
    if (err) {
      response.status(500).json({ error: err.message });
    } else {
      response.status(200).json({ message: 'Withdrawal successful', transaction: dbResult });
    }
  });
});
