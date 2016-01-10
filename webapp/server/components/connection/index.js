var net = require('net');
var tls = require('tls');
var fs = require('fs');
var config = require('../../config/environment');
var wait=require('wait.for');
var parseString = require('xml2js').parseString;

var host = config.ui_server || 4566;
var port = config.ui_server_port || 4566;

var options = {
  pfx: fs.readFileSync('server/config/client.pfx'),
  /*path: '/home/Shu/ui_server-xsutov00@ant-2.fit.vutbr.cz:22'*/
  host: 'localhost',
  port: 51111,
  rejectUnauthorized: false,
  checkServerIdentity : function(servername,cert) { return undefined}
};

var xmlOptions = {explicitRoot:false};

var parseAndCheckResign = function(socket,req,res,message,data,previousCallback,callback){
  var parsedData = wait.for(parseString, data, xmlOptions);
  if(parsedData.$.errcode && parsedData.$.errcode === "20"){
    console.log("NEED RESIGN ->");
    //need to resign
    var protocol = require('../protocol/1.0.js');
    var resignResponse = wait.for(protocol.accounts.login,req,res);
    if(resignResponse.$.errcode){
      console.log("RESIGN NOT SUCCESSFUL errcode:" + resignResponse.$.errcode);
    }else {
      console.log("RESIGN SUCCESSFUL");
    }
    previousCallback(null, resignResponse);
  }else{
    previousCallback(null, parsedData);
  }
};

var parseResponse = function(data,messageCallback,callback){
  var parsedData = wait.for(parseString, data, xmlOptions);
  messageCallback(null,parsedData);
};

module.exports = {
  sendMessage : function(req,res,message,callback){
    var socket = tls.connect(options, function() {
      socket.setTimeout(10*1000);
      if(!socket.authorized){
        console.log('Client unauthorized!!!!!!!!!!!!!!!');
      }
    });
      socket.setEncoding('utf8');
      socket.write(message);
      socket.on('data', function(data) {
        wait.launchFiber(parseResponse,data,callback);
        socket.end();
      });
      socket.on('end', function() {
      });
      socket.on('error', function(err) {
        console.error(err);
        socket.end();
        callback(null,{$:{errcode:"700"}});
      });
  }

};
