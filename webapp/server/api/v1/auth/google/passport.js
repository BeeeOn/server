var passport = require('passport');
var GoogleStrategy = require('passport-google-oauth').OAuth2Strategy;

exports.setup = function ( config) {
	passport.use(new GoogleStrategy({
			clientID: config.google.clientID,
			clientSecret: config.google.clientSecret,
			callbackURL: config.google.callbackURL
		},
		function(accessToken, refreshToken, profile, done) {
			//TODO check user at ui_server
			var user = {
				name: profile.displayName,
				email: profile.emails[0].value,
				role: 'quest',
				username: profile.username,
				provider: 'google',
				google: profile._json
			};
			return done(null,user);
			/*User.findOne({
				'google.id': profile.id
			}, function(err, user) {
				if (!user) {
					user = new User({
						name: profile.displayName,
						email: profile.emails[0].value,
						role: 'user',
						username: profile.username,
						provider: 'google',
						google: profile._json
					});
					user.save(function(err) {
						if (err) done(err);
						return done(err, user);
					});
				} else {
					return done(err, user);
				}
			});*/
		}
	));
};
