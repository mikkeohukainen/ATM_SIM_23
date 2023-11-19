import { Router } from 'express';
import transactions from '../models/transactions_model.js';

export const router = Router();
//const x = 4;
router.get('/:idaccount/:startIndex', (req, res) => {
  const idaccount = parseInt(req.params.idaccount);
  const startIndex = parseInt(req.params.startIndex); // || x;

  transactions.getFiveById(idaccount, startIndex, (err, transactions) => {
    if (err) {
      console.error('Error fetching transactions:', err);
      return res.status(500).json({ error: 'Internal Server Error' });
    }

    res.json(transactions);
  });
});
