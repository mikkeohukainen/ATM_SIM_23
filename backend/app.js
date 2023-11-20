import cookieParser from 'cookie-parser';
import express from 'express';
import logger from 'morgan';
import path from 'path';
import jwt from 'jsonwebtoken';

import { fileURLToPath } from 'url';
import { router as accountRouter } from './routes/account.js';
import { router as account_accessRouter } from './routes/account_access.js';
import { router as customerRouter } from './routes/customer.js';
import { router as transactionRouter } from './routes/transaction.js';
import { router as cardRouter } from './routes/card.js';

import { router as withdrawalRouter } from './routes/withdrawal.js';
import { router as loginRouter } from './routes/login.js';
import { router as transferRouter } from './routes/transfer.js';

import { router as transactionsRouter } from './routes/transactions.js';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/login', loginRouter);

//app.use(authenticateToken);

app.use('/account', accountRouter);
app.use('/customer', customerRouter);
app.use('/account_access', account_accessRouter);
app.use('/transaction', transactionRouter);
app.use('/card', cardRouter);

app.use('/withdrawal', withdrawalRouter);
app.use('/transfer', transferRouter);

app.use('/transactions', transactionsRouter);

function authenticateToken(req, res, next) {
  const authHeader = req.headers['authorization'];
  const token = authHeader && authHeader.split(' ')[1];

  console.log('token = ' + token);
  if (token == null) return res.sendStatus(401);

  jwt.verify(token, process.env.MY_TOKEN, (err, user) => {
    console.log(err);

    if (err) return res.sendStatus(403);

    req.user = user;

    next();
  });
}

export default app;
