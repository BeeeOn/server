'use strict';

var _ = require('lodash');
var demoGateways = require('../../../components/demo/gateways.json');

// Get list of gateways
exports.index = function(req, res) {
  if(req.user.bt === 'demo'){
    /*return res.json({});*/
    return res.json(_.map(demoGateways, function(object) {
      return _.pick(object, ['aid', 'aname','role','utc']);
    }));
  }
  return res.send(500,'Gateways not found!');
};

// Get detailed information about single gateway
exports.show = function(req, res) {
  if(req.user.bt === 'demo'){
    //find gateway and pick certain values
    return res.json(_.pick(_.find(demoGateways, function(g){
      return g.aid === req.params.aid;
    }),
      ['aid', 'role','aname','nfacs','nusers','ip','aversion','utc']
    ));
  }
  return res.send(500,'Gateways not found!');
};

// Update information about gateway
exports.update = function(req, res) {
  if(req.user.bt === 'demo'){
    console.log('DEMO - Trying to change information about gateway: ' + req.params.aid);
    console.log(req.body);
    return res.send(200,'Gateway updated');
  }
  return res.send(500,'Gateways not found!');
};

// Deletes user's access to selected gateway
exports.delete = function(req, res) {
  if(req.user.bt === 'demo'){
    console.log('DEMO - Trying to delete access to gateway: ' + req.params.aid);
    return res.send(200,'Gateway access deleted');
  }
  return res.send(500,'Gateways not found!');
};