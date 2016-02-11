'use strict';

var _ = require('lodash');
var demoUser = require('../../../components/demo/user.json');
var transformer = require('../../../components/transformer');

// Get list of users
exports.index = function(req, res) {
  if(req.user.bt === 'demo'){
    return res.json(demoUser);
  }
  return transformer.doRequest({ns:'accounts',type:'getmyprofile'},req,res);
};
