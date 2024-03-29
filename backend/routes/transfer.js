import { Router } from 'express';
import transfer from '../models/transfer_model.js';

export const router = Router();

router.post('/', function (request, response) {
  const { sendingAccountId, receivingAccountId, amount } = request.body;
  transfer.bankTransfer(sendingAccountId, receivingAccountId, amount, function (err, dbResult) {
    if (err) {
      response.status(500).json({ error: err.message });
    } else {
      response.status(200).json({ message: 'Transfer successful', transaction: dbResult });
    }
  });
});

router.get('/receiver-info/:id', function (request, response) {
  const receivingAccountId = request.params.id;

  transfer.getReceiverInfo(receivingAccountId, function (err, receiverInfo) {
    if (err) {
      response.status(500).json({ error: err.message });
    } else {
      response.json({ receiverName: receiverInfo.name, accountType: receiverInfo.type });
    }
  });
});
