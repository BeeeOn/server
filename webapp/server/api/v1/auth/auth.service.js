'use strict';

var passport = require('passport');
var config = require('../../../config/environment');
var jwt = require('jsonwebtoken');
var expressJwt = require('express-jwt');
var compose = require('composable-middleware');
var validateJwt = expressJwt({ secret: config.secrets.session });

/**
 * Attaches the user object to the request if authenticated
 * Otherwise returns 403
 */
function isAuthenticated() {
	return compose()
		// Validate jwt
		.use(function(req, res, next) {
			// allow access_token to be passed through query parameter as well
			if(req.query && req.query.hasOwnProperty('access_token')) {
				req.headers.authorization = 'Bearer ' + req.query.access_token;
			}
			validateJwt(req, res, next);
		})
		// Attach user to request
		.use(function(req, res, next) {
			next();
			/*User.findById(req.user._id, function (err, user) {
				if (err) return next(err);
				if (!user) return res.send(401);

				req.user = user;
				next();
			});*/
		});
}

/**
 * Checks if the user role meets the minimum requirements of the route
 */
function hasRole(roleRequired) {
	if (!roleRequired) throw new Error('Required role needs to be set');

	return compose()
		.use(isAuthenticated())
		.use(function meetsRequirements(req, res, next) {
			if (config.userRoles.indexOf(req.user.role) >= config.userRoles.indexOf(roleRequired)) {
				next();
			}
			else {
				res.send(403);
			}
		});
}

/**
 * Returns a jwt token signed by the app secret
 */
function signToken(id) {
	return jwt.sign({ bt: id }, config.secrets.session, { expiresInMinutes: 60*24*2 });
}

/**
 * Set token cookie directly for oAuth strategies
 */
function setTokenCookie(req, res) {
	if (!req.bt) return res.json(404, { message: 'Something went wrong, please try again.'});
	var token = signToken(req.bt);
	res.cookie('token', JSON.stringify(token));
	res.redirect('/');
}

exports.isAuthenticated = isAuthenticated;
exports.hasRole = hasRole;
exports.signToken = signToken;
exports.setTokenCookie = setTokenCookie;