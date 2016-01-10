/**
 * Main application routes
 */

'use strict';

var errors = require('./components/errors');
var config = require('./config/environment');


module.exports = function(app) {
  app.all('/*', function(req, res, next) {
    //if in development mode, allow cross origin
    /*if(config.env == 'development'){
      res.header('Access-Control-Allow-Origin', '*');
      res.header('Access-Control-Allow-Headers', 'Cache-Control, Pragma, Origin, Authorization,   Content-Type, X-Requested-With');
      res.header('Access-Control-Allow-Methods', 'POST, GET, OPTIONS, PUT, DELETE, HEAD');
      res._headerNames['access-control-allow-origin'] = 'Access-Control-Allow-Origin';
      res._headerNames['access-control-allow-headers'] = 'Access-Control-Allow-Headers';
      res._headerNames['access-control-allow-methods'] = 'Access-Control-Allow-Methods';
    }*/
    return next();
  });
  // Insert routes below
  app.use('/api/v1/gateway/', require('./api/v1/gateway'));
  app.use('/api/v1/user', require('./api/v1/user'));
  app.use('/api/v1/auth', require('./api/v1/auth'));

  // All undefined asset or api routes should return a 404
  app.route('/:url(api|auth|components|services|app|bower_components|assets)/*')
   .get(errors[404]);

  // All other routes should redirect to the index.html
  app.route('/*')
    .get(function(req, res) {
      res.sendfile(app.get('appPath') + '/index.html');
    });

};
