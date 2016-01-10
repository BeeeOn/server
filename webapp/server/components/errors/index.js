/**
 * Error responses
 */

'use strict';

var config = require('../../config/environment');

module.exports[404] = function pageNotFound(req, res) {
  var viewFilePath = '404';
  var statusCode = 404;
  var result = {
    status: statusCode
  };

  res.status(result.status);
  res.render(viewFilePath, function (err) {
    if (err) { return res.json(result, result.status); }

    res.render(viewFilePath);
  });
};

module.exports[500] = function serverUnavailable(req, res) {
  var viewFilePath = '500';
  var statusCode = 500;
  var result = {
    status: statusCode
  };
  res.set('Location',config.url);
  res.status(result.status);
  console.log(res);
  res.render(viewFilePath, function (err) {
    if (err) { return res.json(result, result.status); }

    res.render(viewFilePath);
  });
};

module.exports['compose'] = function(code,msg){
  return {error : {code:code,msg:msg}};
};
