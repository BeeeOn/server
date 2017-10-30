#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import os

from xmlui import Connector, Response, Login, Logout
from xmlui import FCMTokenRegister, FCMTokenUnregister

class TestFCMTokenRegisterUnregister(unittest.TestCase):
	"""
	Create session for the default user.
	"""
	def setUp(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_data())
		self.session = response.sessionid()

	"""
	Destroy the session.
	"""
	def tearDown(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	"""
	Test register and unregister predefined FCM token.
	"""
	@unittest.skipIf("FCM_AVAILABLE" not in os.environ,
			"fcm support is disabled")
	def test_register_unregister(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)
		response = c.request(FCMTokenRegister(
			config.fcm_token,
			self.session
		))

		self.assertTrue(response.is_ok())

		response = c.request(FCMTokenUnregister(
			config.fcm_token,
			self.session
		))

		self.assertTrue(response.is_ok())

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
