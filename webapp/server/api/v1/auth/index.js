'use strict';

var express = require('express');
var passport = require('passport');
var config = require('../../../config/environment');
var authController = require('./auth.controller.js');
var wait = require('wait.for');

// Passport Configuration
/*require('./local/passport').setup(User, config);
require('./facebook/passport').setup(User, config);*/
require('./google/passport').setup(config);

var router = express.Router();

router.use('/local', require('./local'));
router.use('/logout', require('./logout'));
/*router.use('/facebook', require('./facebook'));*/
router.use('/google', require('./google/index'));
router.post('/register', function(req,res){ wait.launchFiber(authController.register,req,res)});

module.exports = router;
