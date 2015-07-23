'use strict';

var express = require('express');
var controller = require('./gateway.controller');
var auth = require('../auth/auth.service');

var router = express.Router();

router.get('/', auth.isAuthenticated(), controller.index);
router.get('/:aid', auth.isAuthenticated(), controller.show);

module.exports = router;