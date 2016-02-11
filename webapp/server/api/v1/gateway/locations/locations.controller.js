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
    var locations = _.result(_.find(demoGateways, function(g){
      return g.id === req.params.id;
    }),'locations');
    if(locations){
      return res.json(locations);
    }else {
      return res.json([]);
    }
  }
  return transformer.doRequest({ns:'locations',type:'getAll'},req,res);
};
