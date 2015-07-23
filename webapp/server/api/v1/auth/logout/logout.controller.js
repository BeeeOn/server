'use strict';

var _ = require('lodash');

// Get list of logouts
exports.index = function(req, res) {
  //TODO logout from ui_server

  return res.send(200,'Logout successful');
};