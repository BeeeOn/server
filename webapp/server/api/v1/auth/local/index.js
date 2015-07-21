'use strict';

var express = require('express');
var auth = require('../auth.service');

var router = express.Router();

router.post('/', function(req,res,next){
	if(typeof req.body.bt == 'undefined'){
		return res.send(401, "BeeeOnToken not defined");
	}
	var token = null;
	if(req.body.bt === 'demo'){
		token = auth.signToken(req.body.bt);
		return res.json({token:token});
	}
	return res.send(401,'Unauthorized user.')
});

module.exports = router;