#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
import os

import google
import facebook
from rest import POST, DELETE

def login(body):
	req = POST(config.ui_host, config.ui_port, "/auth")
	req.body(body)

	response, data = req()
	result = json.loads(data)

	if result["status"] == "success":
		return response, result["data"]["id"]
	else:
		return response, result["message"]

def logout(session):
	req = DELETE(config.ui_host, config.ui_port, "/auth")
	req.authorize(session)
	return req()

class TestAuth(unittest.TestCase):
	"""
	Test login & logout with the permit provider.
	Disable this test by setting PERMIT_UNAVAILABLE
	environment variable.
	"""
	@unittest.skipIf("PERMIT_UNAVAILABLE" in os.environ,
			"permit provider is disabled")
	def test1_login_logout_permit(self):
		response, session = login(config.PERMIT_LOGIN)
		self.assertEqual(200, response.status)

		response, _ = logout(session)
		self.assertEqual(204, response.status)

	"""
	Test login & logout with the Google provider. The test
	is skipped unless either:

	1) GOOGLE_AUTH_CODE is provided as an environment variable.

	2) GOOGLE_USER, GOOGLE_PASSWORD and GOOGLE_CLIENT_ID are
	   provided as environment variables and oauth2 provides
	   automatic login capabilities (via Selenium).
	"""
	@unittest.skipIf(google.skip_login(), "Missing configuration to perform Google login")
	def test2_login_logout_google(self):
		GOOGLE_LOGIN = json.dumps({
			"provider": "google",
			"code": google.login_auth_code()
		})

		response, session = login(GOOGLE_LOGIN)
		self.assertEqual(200, response.status)

		response, _ = logout(session)
		self.assertEqual(204, response.status)

	"""
	Test login & logout with the Facebook provider.
	Provide the authCode (obtained manually) in the
	environment variable FACEBOOK_AUTH_CODE otherwise
	the test is skipped.
	"""
	@unittest.skipIf(facebook.skip_login(), "Missing configuration to perform Facebook login")
	def test3_login_logout_facebook(self):
		FACEBOOK_LOGIN = json.dumps({
			"provider": "facebook",
			"code": facebook.login_auth_code()
		})

		response, session = login(FACEBOOK_LOGIN)
		self.assertEqual(200, response.status)

		response, _ = logout(session)
		self.assertEqual(204, response.status)

	"""
	Test maximal count of sessions per user.
	The sessions limit is given as config.ui_session_per_user.
	"""
	@unittest.skipIf("PERMIT_UNAVAILABLE" in os.environ,
			"permit provider is disabled")
	def test4_reach_max_session_count(self):
		session = []

		for i in range(0, config.ui_session_per_user):
			response, content = login(config.PERMIT_LOGIN)
			self.assertEqual(200, response.status)
			session.append(content)

		response, message = login(config.PERMIT_LOGIN)
		self.assertEqual(401, response.status)
		self.assertEqual("not authenticated", message)

		for s in session:
			response, _ = logout(s)
			self.assertEqual(204, response.status)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
