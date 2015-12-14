/**
 * Main application file
 */

'use strict';

// Set default node environment to development
process.env.NODE_ENV = process.env.NODE_ENV || 'development';
//require('look').start(5555,'192.168.1.175');
var express = require('express');
var config = require('./config/environment');
// Setup server
var app = express();
var server = require('http').createServer(app);
require('./config/express')(app);
require('./routes')(app);

// Start server
// server.listen(config.port, config.ip, function () {
server.listen(config.port, config.ip, function () {
  console.log('Express server listening on %s %d, in %s mode',config.ip, config.port, app.get('env'));
});

// Expose app
exports = module.exports = app;