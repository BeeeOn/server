'use strict';

var _ = require('lodash');
var demoGateways = require('../../../components/demo/gateways.json');

// Get list of gateways
exports.index = function(req, res) {
  if(req.user.bt === 'demo'){
    return res.json(_.map(demoGateways, function(object) {
      return _.pick(object, ['aid', 'aname','role','utc']);
    }));
  }
  return res.send(500,'Gateways not found!');
};

// Get a single thing
exports.show = function(req, res) {
  if(req.user.bt === 'demo'){
    return res.json(_.find(demoGateways, function(g){
      return g.aid === req.params.aid;
    }));
  }
  return res.send(500,'Gateways not found!');
};