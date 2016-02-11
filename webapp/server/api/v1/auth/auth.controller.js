'use strict';

var _ = require('lodash');
var demoGateways = require('../../../components/demo/gateways.json');
var transformer = require('../../../components/transformer');

// Get list of gateways
exports.register = function(req, res) {
  return transformer.register(req,res);
};
