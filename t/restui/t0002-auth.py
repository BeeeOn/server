#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
import os

import google
import facebook
from rest import GET, POST, DELETE

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
	def find_provider(self, providers, id):
		for p in providers:
			if p["id"] == id:
				return p

		return None

	"""
	List available providers. The result depends on the server configuration
	and thus the test must adapt to it.
	"""
	def test1_list_providers(self):
		req = GET(config.ui_host, config.ui_port, "/auth")
		response, content = req()

		self.assertEqual(200, response.status)

		data = json.loads(content)
		self.assertEqual("success", data["status"])

		providers = data["data"]
		self.assertTrue(len(providers) > 0, "no provider is available")

		no_dups = {}
		for p in providers:
			id = p["id"]
			self.assertFalse(id in no_dups, "duplicate provider %s" % id)
			no_dups[id] = 1

		if not "PERMIT_UNAVAILABLE" in os.environ:
			provider = self.find_provider(providers, "permit")
			self.assertIsNotNone(provider)
			self.assertEqual("permit", provider["name"])
			self.assertEqual("Permit Login", provider["display_name"])

		if not google.skip_login():
			provider = self.find_provider(providers, "google-webapp")
			self.assertIsNotNone(provider)
			self.assertEqual("google", provider["name"])
			self.assertEqual("Google Login", provider["display_name"])
			self.assertTrue("oauth2" in provider)
			self.assertTrue("client_id" in provider["oauth2"])

		if not facebook.skip_login():
			provider = self.find_provider(providers, "facebook-webapp")
			self.assertIsNotNone(provider)
			self.assertEqual("facebook", provider["name"])
			self.assertEqual("Facebook Login", provider["display_name"])
			self.assertTrue("oauth2" in provider)
			self.assertTrue("client_id" in provider["oauth2"])

	"""
	Test login & logout with the permit provider.
	Disable this test by setting PERMIT_UNAVAILABLE
	environment variable.
	"""
	@unittest.skipIf("PERMIT_UNAVAILABLE" in os.environ,
			"permit provider is disabled")
	def test2_login_logout_permit(self):
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
	def test3_login_logout_google_webapp(self):
		GOOGLE_LOGIN = json.dumps({
			"provider": "google-webapp",
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
	def test4_login_logout_facebook_webapp(self):
		FACEBOOK_LOGIN = json.dumps({
			"provider": "facebook-webapp",
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
	def test5_reach_max_session_count(self):
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
