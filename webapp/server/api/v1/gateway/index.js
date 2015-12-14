'use strict';

var express = require('express');
var controller = require('./gateway.controller');
var accountsController = require('./accounts/accounts.controller.js');
var locationsController = require('./locations/locations.controller.js');
var devicesController = require('./devices/devices.controller.js');
var auth = require('../auth/auth.service');

var router = express.Router();

router.get('/', auth.isAuthenticated(), controller.index);
router.get('/:aid', auth.isAuthenticated(), controller.show);
router.put('/:aid', auth.isAuthenticated(), controller.update);
router.delete('/:aid', auth.isAuthenticated(), controller.delete);
/* gateway accounts*/
router.get('/:aid/accounts', auth.isAuthenticated(), accountsController.index);
router.post('/:aid/accounts', auth.isAuthenticated(), accountsController.create);
/* gateway locations */
router.get('/:aid/locations', auth.isAuthenticated(), locationsController.index);
/* gateway devices */
router.get('/:aid/devices', auth.isAuthenticated(), devicesController.index);
router.put('/:aid/devices/:did', auth.isAuthenticated(), devicesController.put);

module.exports = router;