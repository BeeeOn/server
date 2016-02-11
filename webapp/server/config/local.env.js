'use strict';

// Use local.env.js for environment variables that grunt will set when the server starts locally.
// Use for your api keys, secrets, etc. This file should not be tracked by git.
//
// You will need to set these on the server you deploy to.

module.exports = {
  DOMAIN: (process.env.NODE_ENV == 'development') ? process.env.IP+"9000" : 'ant-2.fit.vutbr.cz:9000',
  PORT: '9000',
  IP: '0.0.0.0',
  SESSION_SECRET: "beeeonweb-secret",
  //GOOGLE API AUTH
  GOOGLE_ID: '863203863728-h0fncplfk0fik1grktut8t8n4eh1glvu.apps.googleusercontent.com',
  GOOGLE_SECRET: 'H7vnHuFL7IDJRaBs3VKJLBwa',
  // Control debug level for modules using visionmedia/debug
  DEBUG: ''
};


