import cookieParser from 'cookie-parser';
import express from 'express';
import logger from 'morgan';
import path from 'path';
import { fileURLToPath } from 'url';
import { router as accountRouter } from './routes/account.js';
import { router as customerRouter } from './routes/customer.js';
import { router as indexRouter } from './routes/index.js';
import { router as usersRouter } from './routes/users.js';

const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);

const app = express();

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', indexRouter);
app.use('/users', usersRouter);

app.use('/account', accountRouter);
app.use('/customer', customerRouter);

export default app;
