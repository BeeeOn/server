'use strict';

var express = require('express');
var passport = require('passport');
var config = require('../../../config/environment');

// Passport Configuration
/*require('./local/passport').setup(User, config);
require('./facebook/passport').setup(User, config);*/
require('./google/passport').setup(config);

var router = express.Router();

router.use('/local', require('./local'));
router.use('/logout', require('./logout'));
/*router.use('/facebook', require('./facebook'));*/
router.use('/google', require('./google/index'));

module.exports = router;
