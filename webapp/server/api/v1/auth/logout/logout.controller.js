'use strict';

var _ = require('lodash');
var transformer = require('../../../../components/transformer');

// Get list of logouts
exports.index = function(req, res) {
  if(req.user.bt === 'demo'){
    return res.send(200);
  }
  return transformer.doRequest({ns:'accounts',type:'logout'},req,res);
};
