'use strict';

var express = require('express');
var controller = require('./logout.controller');
var auth = require('../../auth/auth.service');

var router = express.Router();

router.post('/', auth.isAuthenticated(), controller.index);

module.exports = router;