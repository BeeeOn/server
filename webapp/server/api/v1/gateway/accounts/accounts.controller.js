'use strict';

var _ = require('lodash');
var demoGateways = require('../../../../components/demo/gateways.json');
var transformer = require('../../../../components/transformer');

// Get list of accounts
exports.index = function(req, res) {
  if(!req.params.id){
    //missing gateway number
    return res.send(500,{error: "5"});
  }
  if(req.user.bt === 'demo'){
    //find gateway and get it's accounts
    var accounts = _.result(_.find(demoGateways, function(g){
      return g.id === req.params.id;
    }),'accounts');
    if(accounts){
      return res.json(accounts);
    }else {
      return res.send(404,'Wrong gateway number');
    }
  }
  return transformer.doRequest({ns:'gateusers',type:'getAll'},req,res);
};

exports.create = function(req, res) {
  if(!req.params.id){
    //missing gateway number
    return res.send(500,{error: "5"});
  }
  if(req.user.bt === 'demo'){
    console.log('DEMO - Trying to add account to gateway:' + req.params.id);
    console.log(req.body);
    //return res.send(401,'Unauthorized');
    return res.send(200,'Success');

  }
  return transformer.doRequest({ns:'gateusers',type:'invite'},req,res);
};
