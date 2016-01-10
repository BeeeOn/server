'use strict';

var _ = require('lodash');
var demoGateways = require('../../../../components/demo/gateways.json');
var transformer = require('../../../../components/transformer');

// Get list of devicess
exports.index = function(req, res) {
  if(!req.params.id){
    //missing gateway number
    return res.send(500,{error: "5"});
  }
  if(req.user.bt === 'demo'){
    //find gateway and get it's accounts
    var devices = _.result(_.find(demoGateways, function(g){
      return g.id === req.params.id;
    }),'devices');
    if(devices){
      return res.json(devices);
    }else {
      return res.json([]);
    }
  }
  return transformer.doRequest({ns:'devices',type:'getAll'},req,res);
};

// Update information about gateway
exports.put = function(req, res) {
  if(req.user.bt === 'demo'){
    /*return res.send(404,'Wrong gateway number');*/
    console.log('DEMO - Trying to change state on gateway='+req.params.id+', device='+req.params.did);
    console.log(req.body);
    return res.send(200,'Module state updated');
  }
  return res.send(500,'Wrong comm. protocol version');
};
