'use strict';

var express = require('express');
var controller = require('./logout.controller');
var auth = require('../../auth/auth.service');
var wait=require('wait.for');

var router = express.Router();

router.post('/', auth.isAuthenticated(), function(req,res){ wait.launchFiber(controller.index,req,res)});

module.exports = router;
