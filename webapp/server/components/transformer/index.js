var config = require('../../config/environment');
var protocol = require('../protocol/'+ (config.ui_protocol || "1.0") +'.js');
var error = require('../errors');

var functionMap = {
  "gates" : {
    "getAll" :{
      action : protocol.gates.getAll
    },
    "register" : {
      action : protocol.gates.register
    },
    "get" :{
      action : protocol.gates.get
    },
    "update" : {
      action : protocol.gates.update
    },
    "unregister" : {
      action : protocol.gates.unregister
    }
  },
  "devices" : {
    "getAll" :{
      action : protocol.devices.getAll
    }
  },
  "locations" : {
    "getAll" :{
      action : protocol.locations.getAll
    }
  },
  "accounts" : {
    "login":{
      action: protocol.accounts.login
    },
    "register":{
      action: protocol.accounts.register
    },
    "getmyprofile":{
      action: protocol.accounts.getmyprofile
    },
    "logout" : {
      action: protocol.accounts.logout
    }
  },
  "gateusers" : {
    "getAll" :{
      action : protocol.gateusers.getAll
    },
    "invite" :{
      action : protocol.gateusers.invite
    }
  }
};
module.exports = {
  doRequest : function(message,req,res){
    if(!message.ns || !message.type){
      //only for debug, check if request is known
      return res.send(500,"Unknown request type!(No message namespace or type)");
    }
    if(!protocol){
      return res.send(500,'Wrong protocol number. Please contact administrator!');
    }
    //find message in functionMap by namespace and message type
    var found = functionMap[message.ns];
    if(found){
      found = functionMap[message.ns][message.type];
      if(found){
        var ui_response = found.action(req,res);
        if(ui_response.error){
          return res.send(500,{errcode:ui_response.error})
        }else{
          return res.json(ui_response);
        }
      }
    }
    return res.send(500,"Unknown message type. (Not in table)");
  },
  login : function(req,res,redirect){
    var ui_response = functionMap["accounts"]["login"].action(req,res);
    if(ui_response.error){
      if(ui_response.error === "24"){
        res.redirect('/login?error='+ui_response.error+'&token='+req.user.access_token+'&provider='+req.user.provider);
      }else if(ui_response.error === "700") {
        res.redirect('/login?error='+ui_response.error);
      }else{
        return res.send(500,{errcode:ui_response.error});
      }
    }else{
      console.log("RESPOSNE: " + JSON.stringify(ui_response));
      if(redirect){
        res.redirect('/');
      }else{
        return res.json(ui_response);
      }
    }
  },
  register : function(req,res){
    var ui_response = functionMap["accounts"]["register"].action(req,res);
    if(ui_response.error){
      res.redirect('/login?error='+ui_response.error);
    }else{
      console.log("RESPOSNE: " + JSON.stringify(ui_response));
      return res.json(ui_response);
    }
  }

};
