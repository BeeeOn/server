'use strict';

var _ = require('lodash');
var demoGateways = require('../../../../components/demo/gateways.json');

// Get list of devicess
exports.index = function(req, res) {
  if(req.user.bt === 'demo'){
    //find gateway and get it's accounts
    var devices = _.result(_.find(demoGateways, function(g){
      return g.aid === req.params.aid;
    }),'devices');
    if(devices){
      return res.json(devices);
    }else {
      return res.send(404,'Wrong gateway number');
    }
  }
  return res.send(500,'Wrong comm. protocol version');
};

// Update information about gateway
exports.put = function(req, res) {
  if(req.user.bt === 'demo'){
    /*return res.send(404,'Wrong gateway number');*/
    console.log('DEMO - Trying to change state on gateway='+req.params.aid+', device='+req.params.did);
    console.log(req.body);
    return res.send(200,'Module state updated');
  }
  return res.send(500,'Wrong comm. protocol version');
};