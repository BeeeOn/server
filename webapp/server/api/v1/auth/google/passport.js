var passport = require('passport');
var GoogleStrategy = require('passport-google-oauth').OAuth2Strategy;
var transformer = require('../../../../components/transformer');
var wait=require('wait.for');

exports.setup = function ( config) {
	passport.use(new GoogleStrategy({
			clientID: config.google.clientID,
			clientSecret: config.google.clientSecret,
			callbackURL: config.google.callbackURL
		},
		function(accessToken, refreshToken, profile, done) {
			var user = {
        provider: 'google',
        access_token: accessToken
			};
			return done(null,user);
		}
	));
};
