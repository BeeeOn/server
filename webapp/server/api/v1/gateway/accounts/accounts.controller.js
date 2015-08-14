'use strict';

var _ = require('lodash');
var demoGateways = require('../../../../components/demo/gateways.json');

// Get list of accounts
exports.index = function(req, res) {
  if(req.user.bt === 'demo'){
    //find gateway and get it's accounts
    var accounts = _.result(_.find(demoGateways, function(g){
      return g.aid === req.params.aid;
    }),'accounts');
    if(accounts){
      return res.json(accounts);
    }else {
      return res.send(404,'Wrong gateway number');
    }
  }
  return res.send(500,'Wrong comm. protocol version');
};

exports.create = function(req, res) {
  if(req.user.bt === 'demo'){
    console.log('Adding account to gateway:' + req.params.aid);
    console.log(req.body);
    //return res.send(401,'Unauthorized');
    return res.send(200,'Success');

  }
  return res.send(500,'Wrong comm. protocol version');
};