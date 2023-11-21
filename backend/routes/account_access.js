import { Router } from 'express';
import account_access from '../models/account_access_model.js';

export const router = Router();

router.get('/', function (request, response) {
  account_access.getAll(function (err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      console.log(dbResult);
      response.json(dbResult);
    }
  });
});

router.get('/:id', function (request, response) {
  account_access.getByAccountId(request.params.id, function (err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult[0]);
    }
  });
});

router.post('/', function (request, response) {
  account_access.add(request.body, function (err) {
    if (err) {
      response.json(err);
    } else {
      response.json(request.body);
    }
  });
});

router.delete('/:id', function (request, response) {
  account_access.delete(request.params.id, function (err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});

router.put('/:id', function (request, response) {
  account_access.update(request.params.id, request.body, function (err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});
