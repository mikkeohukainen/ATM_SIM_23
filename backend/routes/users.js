import { Router } from 'express';

export const router = Router();

/* GET users listing. */
router.get('/', function (_, res) {
  res.send('respond with a resource');
});
