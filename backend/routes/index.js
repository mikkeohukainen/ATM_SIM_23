import { Router } from 'express';

export const router = Router();

/* GET home page. */
router.get('/', function (_, res) {
  res.render('index', { title: 'Express' });
});
