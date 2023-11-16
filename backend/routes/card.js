import { Router } from 'express';
import card from '../models/card_model.js';

export const router = Router();

router.get('/', function (request, response) {
  card.getAll(function (err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});

router.get('/:id', function (request, response) {
  card.getById(request.params.id, function (err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult[0]);
    }
  });
});

router.post('/', function (request, response) {
  card.add(request.body, function (err) {
    if (err) {
      response.json(err);
    } else {
      response.json(request.body);
    }
  });
});

router.delete('/:id', function (request, response) {
  card.delete(request.params.id, function (err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});

router.put('/:id', function (request, response) {
  card.update(request.params.id, request.body, function (err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});

router.put('/hash/:id', function (request, response) {
  card.hashPin(request.params.id, request.body, function (err, dbResult) {
    if (err) {
      response.json(err);
    } else {
      response.json(dbResult);
    }
  });
});
