'use strict';

var express = require('express');
var auth = require('../auth.service');
var wait=require('wait.for');
var transformer = require('../../../../components/transformer');
var router = express.Router();
var config = require('../../../../config/environment');
var expressJwt = require('express-jwt');
var validateJwt = expressJwt({ secret: config.secrets.session });

var getTokenFromJWT = function(req,res,next){
  // allow access_token to be passed through query parameter as well
  if(req.query && req.query.hasOwnProperty('access_token')) {
    req.headers.authorization = 'Bearer ' + req.query.access_token;
    console.log(req.headers.authorization);
  }
  validateJwt(req, res, next);
};

router.post('/', function(req,res,next){ wait.launchFiber(
  function(req,res,next){
    if(typeof req.body.provider == 'undefined'){
      return res.send(401, "Unknown authorization provider");
    }
    var token = null;
    if(req.body.provider === 'demo'){
      token = auth.signToken('demo',req.body.provider,'');
      return res.json({token:token});
    }else if(req.body.provider === 'google'){
      if(req.body.token){
        console.log("found token in local auth");
        req.user = req.body;
      }else{
        getTokenFromJWT(req,res,next);
      }
      return transformer.login(req,res,false);
    }else{
      return res.send(401,"Cannot login with " + req.body.provider + " (missing information)");
    }
  },req,res,next)}
);


module.exports = router;
