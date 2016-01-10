'use strict';

var express = require('express');
var controller = require('./gateway.controller');
var accountsController = require('./accounts/accounts.controller.js');
var locationsController = require('./locations/locations.controller.js');
var devicesController = require('./devices/devices.controller.js');
var auth = require('../auth/auth.service');
var wait=require('wait.for');

var router = express.Router();

router.get('/', auth.isAuthenticated(), function(req,res){ wait.launchFiber(controller.index,req,res)});
router.post('/', auth.isAuthenticated(), function(req,res){ wait.launchFiber(controller.register,req,res)});
router.get('/:id', auth.isAuthenticated(), function(req,res){ wait.launchFiber(controller.show,req,res)});
router.put('/:id', auth.isAuthenticated(), function(req,res){ wait.launchFiber(controller.update,req,res)});
router.delete('/:id', auth.isAuthenticated(), function(req,res){ wait.launchFiber(controller.delete,req,res)});
/* gateway accounts*/
router.get('/:id/accounts', auth.isAuthenticated(), function(req,res){ wait.launchFiber(accountsController.index,req,res)});
router.post('/:id/accounts', auth.isAuthenticated(), function(req,res){ wait.launchFiber(accountsController.create,req,res)});
/* gateway locations */
router.get('/:id/locations', auth.isAuthenticated(), function(req,res){ wait.launchFiber(locationsController.index,req,res)});
/* gateway devices */
router.get('/:id/devices', auth.isAuthenticated(), function(req,res){ wait.launchFiber(devicesController.index,req,res)});
router.put('/:id/devices/:did', auth.isAuthenticated(), function(req,res){ wait.launchFiber(devicesController.put,req,res)});

module.exports = router;
