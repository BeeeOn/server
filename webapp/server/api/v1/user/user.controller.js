'use strict';

var _ = require('lodash');
var demoUser = require('../../../components/demo/user.json');

// Get list of users
exports.index = function(req, res) {
  if(req.user.bt === 'demo'){
    return res.json(demoUser);
  }
  return res.send(500,'User not found!');
};