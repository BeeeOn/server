var errors = require('../errors');
var connection = require('../connection');
var wait=require('wait.for');
var auth = require('../../api/v1/auth/auth.service.js');
var _ = require('lodash');

var messageHead = '<?xml version=1.0 encoding="UTF-8"?>';
var parseString = require('xml2js').parseString;

var protocolNumber = "1.0.0";

var extractData = function(object){

};

var protocol = {
  gates : {
    getAll : function(req,res){
      var message = '<request version="'+protocolNumber+'" ns="gates" type="getall" sessionid="'+req.user.bt+'"></request>';
      console.log("GATES GETALL MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log(parsedResponse);
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : _.pluck(parsedResponse.gate,'$'));
    },
    register : function(req,res){
      var message = '<request version="'+protocolNumber+'" ns="gates" type="register" sessionid="'+req.user.bt+'"><gate id="'+req.body.id+'" name="'+req.body.name+'" timezone="'+req.body.timezone+'" /></request>';
      console.log("REGISTER GATEWAY MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log(parsedResponse);
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : {});
      //return {result:parsedResponse.$.result};
    },
    get : function(req,res){
      var message = '<request version="'+protocolNumber+'" ns="gates" type="get" sessionid="'+req.user.bt+'"><gate id="'+req.params.id+'" /></request>';
      console.log("GATES GET MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      var info = _.pluck(parsedResponse.gate,'$')[0];
      if(!info.devices && !parsedResponse.$.errcode){
        return {error: "700"};
      }
      info.ndevices = info.devices;
      delete info.devices;
      console.log(info);
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : info);
    },
    update : function(req,res){
      var message = '<request version="'+protocolNumber+'" ns="gates" type="update" sessionid="'+req.user.bt+'">' +
        '<gate id="'+req.params.id+'" name="'+req.body.name+'" timezone="'+req.body.timezone+'" />' +
        '</request>';
      console.log("GATES GET MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : {result:parsedResponse.$.result});
    },
    unregister : function(req,res){
      var message = '<request version="'+protocolNumber+'" ns="gates" type="unregister" sessionid="'+req.user.bt+'"><gate id="'+req.params.id+'" /></request>';
      console.log("UNREGISTER GATEWAY MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log(parsedResponse);
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : {result:parsedResponse.$.result});
      //return {result:parsedResponse.$.result};
    }
  },
  accounts : {
    login : function(req,res){
      var message;
      if(req.user.provider === 'google'){
        message = '<request version="'+protocolNumber+'" ns="accounts" type="login">'+'<provider name="'+req.user.provider+'" token="'+req.user.access_token+'" /></request>';
      }
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log("PARSEDMESSAGE:");
      console.log(parsedResponse);
      if(parsedResponse.$.errcode){
        return {error: parsedResponse.$.errcode};
      }
      req.user.bt = parsedResponse.session[0].$.id;
      return {token:auth.setTokenCookie(req,res)};
    },
    register : function(req,res){
      var message = '<request version="'+protocolNumber+'" ns="accounts" type="register">'+'<provider name="'+req.body.provider+'" token="'+req.body.token+'" /></request>';
      console.log("REGISTER MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log("PARSEDMESSAGE:");
      console.log(parsedResponse);
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : {result:parsedResponse.$.result});
    },
    getmyprofile : function(req,res){
      var message = '<request version="'+protocolNumber+'" ns="accounts" type="getmyprofile" sessionid="'+req.user.bt+'"></request>';
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log("PARSEDMESSAGE:");
      if(parsedResponse.$.errcode){
        return {error: parsedResponse.$.errcode};
      }
      var user = parsedResponse.user[0].$;
      user.providers = [];
      _.forEach(parsedResponse.providers,function(n,key){
        n = _.pluck(n.provider,'$');
        n = n[0];
        user.providers.push(n);
      });
      user.accounts = [];
      return user;
    },
    logout : function(req,res){
      var message = '<request version="'+protocolNumber+'" ns="accounts" type="logout" sessionid="'+req.user.bt+'"></request>';
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log("PARSEDMESSAGE:");
      console.log(parsedResponse);
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : {result:parsedResponse.$.result});
    }
  },
  locations : {
    getAll : function(req,res) {
      if(!req.params.id){
        //missing gateway number
        return {error: "5"};
      }
      var message = '<request version="'+protocolNumber+'" ns="locations" type="getall" sessionid="'+req.user.bt+'" gateid="'+req.params.id+'"></request>';
      console.log("LOCATIONS GETALL MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log(_.pluck(parsedResponse.location,'$'));
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : _.pluck(parsedResponse.location,'$'));
    }
  },
  devices : {
    getAll : function(req,res) {
      if(!req.params.id){
        //missing gateway number
        return {error: "5"};
      }
      var message = '<request version="'+protocolNumber+'" ns="devices" type="getall" sessionid="'+req.user.bt+'" gateid="'+req.params.id+'"></request>';
      console.log("DEVICES GETALL MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      var devices = [];
      _.forEach(parsedResponse.device,function(n,key){
        //extract modules
        n.$.modules = _.pluck(n.module,'$');
        //change euid to id
        n.$.id = n.$.euid;
        //set location, if doesn't exist
        if(!n.$.locationid){
          n.$.locationid = '0';
        }
        devices.push(n.$);
      });
      console.log(devices);
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : devices);
    }
  },
  gateusers : {
    getAll : function(req,res) {
      var message = '<request version="'+protocolNumber+'" ns="gateusers" type="getall" sessionid="'+req.user.bt+'" gateid="'+req.params.id+'"></request>';
      console.log("GATEUSERS GETALL MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log(_.pluck(parsedResponse.user,'$'));
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : _.pluck(parsedResponse.user,'$'));
    },
    invite : function(req,res){
      var message = '<request version="'+protocolNumber+'" ns="gateusers" type="invite" sessionid="'+req.user.bt+'"  gateid="'+req.params.id+'">' +
        '<user email="'+req.body.email+'" permission="'+req.body.permission+'" />' +
        '</request>';
      console.log("GATEUSERS INVITE MESSAGE: " + message);
      var parsedResponse = wait.for(connection.sendMessage,req,res,messageHead + message);
      console.log("PARSEDMESSAGE:");
      console.log(parsedResponse);
      return (parsedResponse.$.errcode ? {error: parsedResponse.$.errcode} : {result:parsedResponse.$.result});
    }
  }

};

module.exports = protocol;
