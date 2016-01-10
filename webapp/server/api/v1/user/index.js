'use strict';

var express = require('express');
var controller = require('./user.controller');
var config = require('../../../config/environment');
var auth = require('../auth/auth.service');
var wait=require('wait.for');

var router = express.Router();

router.get('/',auth.isAuthenticated(), function(req,res){ wait.launchFiber(controller.index,req,res)});

module.exports = router;
