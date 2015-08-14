'use strict';

var express = require('express');
var controller = require('./gateway.controller');
var accountsController = require('./accounts/accounts.controller.js');
var auth = require('../auth/auth.service');

var router = express.Router();

router.get('/', auth.isAuthenticated(), controller.index);
router.get('/:aid', auth.isAuthenticated(), controller.show);
router.put('/:aid', auth.isAuthenticated(), controller.update);
router.delete('/:aid', auth.isAuthenticated(), controller.delete);
router.get('/:aid/accounts', auth.isAuthenticated(), accountsController.index);
router.post('/:aid/accounts', auth.isAuthenticated(), accountsController.create);

module.exports = router;