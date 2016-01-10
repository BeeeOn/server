'use strict';

var path = require('path');
var _ = require('lodash');

function requiredProcessEnv(name) {
  if(!process.env[name]) {
    throw new Error('You must set the ' + name + ' environment variable');
  }
  return process.env[name];
}

// All configurations will extend these options
// ============================================
var all = {
  env: process.env.NODE_ENV,

  // Root path of server
  root: path.normalize(__dirname + '/../../..'),

  // Server port
  port: process.env.PORT || 9000,

  // Should we populate the DB with sample data?
  seedDB: false,

  // Secret for session, you will want to change this and make it an environment variable
  secrets: {
    session: '(!%zftrd-$k*f*3aqf)*wua8zd(d!b083j3gf6#l!e*y))=-*+'
  },

  // List of user roles
  userRoles: ['guest', 'user', 'admin','superuser'],

  google: {
    clientID:     process.env.GOOGLE_ID || 'id',
    clientSecret: process.env.GOOGLE_SECRET || 'secret',
    callbackURL:  (process.env.DOMAIN || '') + '/api/v1/auth/google/callback'
  }

};
// Export the config object based on the NODE_ENV
// ==============================================
module.exports = _.merge(
  all,
  require('./' + process.env.NODE_ENV + '.js') || {});

all.google.callbackURL = all.env == 'development' ? all.ip+':'+all.port : 'ant-2.fit.vutbr.cz:9000';
all.google.callbackURL = 'http://' + all.google.callbackURL + '/api/v1/auth/google/callback';
