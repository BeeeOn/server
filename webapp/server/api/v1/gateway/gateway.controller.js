'use strict';

var _ = require('lodash');
var demoGateways = require('../../../components/demo/gateways.json');
var transformer = require('../../../components/transformer');

// Get list of gateways
exports.index = function(req, res) {
  if(req.user.bt === 'demo'){
    return res.json(_.map(demoGateways, function(object) {
      return _.pick(object, ['id', 'name','permission','utc']);
    }));
  }
  return transformer.doRequest({ns:'gates',type:'getAll'},req,res);
};

// Update information about gateway
exports.register = function(req, res) {
  if(req.user.bt === 'demo'){
    return res.send(401,'Demo user cannot add new gateway');
  }
  return transformer.doRequest({ns:'gates',type:'register'},req,res);
};

// Get detailed information about single gateway
exports.show = function(req, res) {
  if(!req.params.id){
    //missing gateway number
    return res.send(500,{error: "5"});
  }
  if(req.user.bt === 'demo'){
    //find gateway and pick certain values
    return res.json(_.pick(_.find(demoGateways, function(g){
      return g.id === req.params.id;
    }),
      ['id', 'permission','owner','name','ndevices','users','ip','version','timezone']
    ));
  }
  return transformer.doRequest({ns:'gates',type:'get'},req,res);
};

// Update information about gateway
exports.update = function(req, res) {
  if(!req.params.id){
    //missing gateway number
    return res.send(500,{error: "5"});
  }
  if(req.user.bt === 'demo'){
    console.log('DEMO - Trying to change information about gateway: ' + req.params.id);
    console.log(req.body);
    return res.send(200,'Gateway updated');
  }
  return transformer.doRequest({ns:'gates',type:'update'},req,res);
};

// Deletes user's access to selected gateway
exports.delete = function(req, res) {
  if(!req.params.id){
    //missing gateway number
    return res.send(500,{error: "5"});
  }
  if(req.user.bt === 'demo'){
    console.log('DEMO - Trying to delete access to gateway: ' + req.params.id);
    return res.send(200,'Gateway access deleted');
  }
  return transformer.doRequest({ns:'gates',type:'unregister'},req,res);
};
