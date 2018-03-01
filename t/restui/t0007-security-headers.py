#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import GET

class TestSecurityHeaders(unittest.TestCase):
	def test1_auth_returns_strict_transport_security(self):
		req = GET(config.ui_host, config.ui_port, "/auth")
		response, _ = req()

		self.assertEqual(200, response.status)
		self.assertIn("Strict-Transport-Security", response.headers)
		self.assertEqual("max-age=31536000; includeSubDomains",
				response.headers["Strict-Transport-Security"])

	def test2_auth_returns_referrer_policy(self):
		req = GET(config.ui_host, config.ui_port, "/auth")
		response, _ = req()

		self.assertIn("Referrer-Policy", response.headers)
		self.assertEqual("no-referrer-when-downgrade", response.headers["Referrer-Policy"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
