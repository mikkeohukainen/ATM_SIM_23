import { Router } from 'express';
import login from '../models/login_model.js';
import bcrypt from 'bcryptjs';
import jwt from 'jsonwebtoken';
import dotenv from 'dotenv';

export const router = Router();

router.post('/', function (request, response) {
  if (request.body.idcard && request.body.pin) {
    const idcard = request.body.idcard;
    const pin = request.body.pin;
    login.checkPin(idcard, function (dbError, dbResult) {
      if (dbError) {
        response.json(dbError);
      } else {
        if (dbResult.length > 0) {
          bcrypt.compare(pin, dbResult[0].pin, function (err, compareResult) {
            if (compareResult) {
              console.log('success');
              const token = generateAccessToken({ idcard: idcard });
              response.send(token);
            } else {
              console.log('wrong pin');
              response.send(false);
            }
          });
        } else {
          console.log('user does not exists');
          response.send(false);
        }
      }
    });
  } else {
    console.log('idcard or pin missing');
    response.send(false);
  }
});

function generateAccessToken(idcard) {
  dotenv.config();
  return jwt.sign(idcard, process.env.MY_TOKEN, { expiresIn: '1800s' });
}
