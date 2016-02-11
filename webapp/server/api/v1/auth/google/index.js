'use strict';

var express = require('express');
var passport = require('passport');
var transformer = require('../../../../components/transformer');
var wait = require('wait.for');

var router = express.Router();
router
	.get('/', passport.authenticate('google', {
		failureRedirect: '/login',
		scope: [
			'https://www.googleapis.com/auth/userinfo.profile',
			'https://www.googleapis.com/auth/userinfo.email'
		],
		session: false
	}))

	.get('/callback', passport.authenticate('google', {
		failureRedirect: '/login',
		session: false
	}),function(req,res){
    wait.launchFiber(transformer.login,req,res,true);
  });


module.exports = router;
