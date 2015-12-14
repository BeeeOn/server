'use strict';

var _ = require('lodash');
var demoGateways = require('../../../../components/demo/gateways.json');

// Get list of accounts
exports.index = function(req, res) {
  if(req.user.bt === 'demo'){
    //find gateway and get it's accounts
    var locations = _.result(_.find(demoGateways, function(g){
      return g.aid === req.params.aid;
    }),'locations');
    if(locations){
      return res.json(locations);
    }else {
      return res.send(404,'Wrong gateway number');
    }
  }
  return res.send(500,'Wrong comm. protocol version');
};