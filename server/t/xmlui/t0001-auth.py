#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import os

from xmlui import Connector, Response, Login, Logout

def login(login):
	c = Connector(config.xmlui_host, config.xmlui_port)
	response = c.request(login)

	if response.is_error():
		return False, response.error_code()

	if not response.is_data():
		raise Exception("unexpected result of login: " + str(response))

	return True, response.sessionid()

def logout(session):
	c = Connector(config.xmlui_host, config.xmlui_port)
	response = c.request(Logout(session))

	if response.is_error():
		return False, response.error_code()

	return response.is_ok()

class TestAuth(unittest.TestCase):
	"""
	Test login & logout with the permit provider.
	Disable this test by setting PERMIT_UNAVAILABLE
	environment variable.
	"""
	@unittest.skipIf("PERMIT_UNAVAILABLE" in os.environ,
			"permit provider is disabled")
	def test1_login_logout_permit(self):
		ok, session = login(config.PERMIT_LOGIN)
		self.assertTrue(ok)
		self.assertTrue(logout(session))

	"""
	Test login & logout with the Google provider.
	Provide the authCode (obtained manually) in the
	environment variable GOOGLE_AUTH_CODE otherwise
	the test is skipped.
	"""
	@unittest.skipIf(not "GOOGLE_AUTH_CODE" in os.environ,
			"no GOOGLE_AUTH_CODE specified")
	def test2_login_logout_google(self):
		GOOGLE_LOGIN = Login("google", os.environ["GOOGLE_AUTH_CODE"])

		ok, session = login(GOOGLE_LOGIN)
		self.assertTrue(ok)
		self.assertTrue(logout(session))

	"""
	Test login & logout with the Facebook provider.
	Provide the authCode (obtained manually) in the
	environment variable FACEBOOK_AUTH_CODE otherwise
	the test is skipped.
	"""
	@unittest.skipIf(not "FACEBOOK_AUTH_CODE" in os.environ,
			"no FACEBOOK_AUTH_CODE specified")
	def test3_login_logout_facebook(self):
		FACEBOOK_LOGIN = Login("facebook", os.environ["FACEBOOK_AUTH_CODE"])

		ok, session = login(FACEBOOK_AUTH_CODE)
		self.assertTrue(ok)
		self.assertTrue(logout(session))

	"""
	Test maximal count of sessions per user.
	The sessions limit is given as config.xmlui_session_per_user.
	"""
	@unittest.skipIf("PERMIT_UNAVAILABLE" in os.environ,
			"permit provider is disabled")
	def test4_reach_max_session_count(self):
		session = []

		for i in range(0, config.xmlui_session_per_user):
			ok, id = login(config.PERMIT_LOGIN)
			self.assertTrue(ok)
			session.append(id)

		ok, code = login(config.PERMIT_LOGIN)
		self.assertFalse(ok)
		self.assertEqual("998", code) # FIXME: different code?

		for s in session:
			self.assertTrue(logout(s))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
