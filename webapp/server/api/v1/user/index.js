'use strict';

var express = require('express');
var controller = require('./user.controller');
var config = require('../../../config/environment');
var auth = require('../auth/auth.service');

var router = express.Router();

router.get('/',auth.isAuthenticated(), controller.index);

module.exports = router;