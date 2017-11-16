#! /usr/bin/env python3

from oauth2client.client import OAuth2WebServerFlow
import sys

"""
Construct the login URI. Pass the URI to your webbrowser to
obtain the _code_ (authorization_code) for sending to the UI
server. The _code_ is returned in URI.
"""
def get_login_uri(client_id, scope):
	redirect_uri = "https://localhost/"
	auth_uri = "https://www.facebook.com/v2.9/dialog/oauth"

	flow = OAuth2WebServerFlow(client_id,
			scope = scope,
			redirect_uri = redirect_uri,
			auth_uri = auth_uri)
	return flow.step1_get_authorize_url()

# require client_id
if len(sys.argv) < 2:
	print("Usage: %s <client_id> [<scope0>,<scope1>,...]" % (sys.argv[0]))
	sys.exit(1)

client_id = sys.argv[1]

# configure scopes
if len(sys.argv) > 2:
	scope = sys.argv[2].split(",")
else:
	scope = ["public_profile", "email"]

print(get_login_uri(client_id, scope))
