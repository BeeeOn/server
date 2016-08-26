#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
import os

from rest import POST, DELETE

def login(body):
	req = POST(config.ui_host, config.ui_port, "/auth")
	req.body(body)
	return req()

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
		response = login(config.PERMIT_LOGIN)
		self.assertEqual(200, response.status)

		session = str(response.read(), "utf-8")

		response = logout(session)
		self.assertEqual(200, response.status)

	"""
	Test login & logout with the Google provider.
	Provide the authCode (obtained manually) in the
	environment variable GOOGLE_AUTH_CODE otherwise
	the test is skipped.
	"""
	@unittest.skipIf(not "GOOGLE_AUTH_CODE" in os.environ,
			"no GOOGLE_AUTH_CODE specified")
	def test2_login_logout_google(self):
		GOOGLE_LOGIN = json.dumps({
			"provider": "google",
			"authCode": os.environ["GOOGLE_AUTH_CODE"]
		})

		response = login(GOOGLE_LOGIN)
		self.assertEqual(200, response.status)

		session = str(response.read(), "utf-8")

		response = logout(session)
		self.assertEqual(200, response.status)

	"""
	Test login & logout with the Facebook provider.
	Provide the authCode (obtained manually) in the
	environment variable FACEBOOK_AUTH_CODE otherwise
	the test is skipped.
	"""
	@unittest.skipIf(not "FACEBOOK_AUTH_CODE" in os.environ,
			"no FACEBOOK_AUTH_CODE specified")
	def test3_login_logout_facebook(self):
		FACEBOOK_LOGIN = json.dumps({
			"provider": "facebook",
			"authCode": os.environ["FACEBOOK_AUTH_CODE"]
		})

		response = login(FACEBOOK_LOGIN)
		self.assertEqual(200, response.status)

		session = str(response.read(), "utf-8")

		response = logout(session)
		self.assertEqual(200, response.status)

	"""
	Test maximal count of sessions per user.
	The sessions limit is given as config.ui_session_per_user.
	"""
	@unittest.skipIf("PERMIT_UNAVAILABLE" in os.environ,
			"permit provider is disabled")
	def test4_reach_max_session_count(self):
		session = []

		for i in range(0, config.ui_session_per_user):
			response = login(config.PERMIT_LOGIN)
			self.assertEqual(200, response.status)
			session.append(str(response.read(), "utf-8"))

		response = login(config.PERMIT_LOGIN)
		self.assertEqual(401, response.status)

		for s in session:
			response = logout(s)
			self.assertEqual(200, response.status)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
