import { Router } from 'express';
import login from '../models/login_model.js';
import bcrypt from 'bcryptjs';
import jwt from 'jsonwebtoken';
import dotenv from 'dotenv';

export const router = Router();

router.post('/', function (request, response) {
  const idcard = request.body.idcard;
  const pin = request.body.pin;

  if (!idcard || !pin) {
    console.log('idcard or pin missing');
    return response.send(false);
  }

  login.getPin(idcard, function (dbError, dbResult) {
    if (dbError) {
      console.log(dbError.errno);
      return response.json(dbError.errno);
    }

    if (dbResult.length === 0) {
      console.log('user does not exist');
      return response.send(false);
    }

    bcrypt.compare(pin, dbResult[0].pin, function (_, compareResult) {
      if (!compareResult) {
        console.log('wrong pin');
        return response.send(false);
      }
      console.log('success');
      const token = generateAccessToken({ idcard: idcard });
      response.send(token);
    });
  });
});

function generateAccessToken(idcard) {
  dotenv.config();
  return jwt.sign(idcard, process.env.MY_TOKEN, { expiresIn: '1800s' });
}
