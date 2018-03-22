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

	def test3_auth_returns_nosniff(self):
		req = GET(config.ui_host, config.ui_port, "/auth")
		response, _ = req()

		self.assertIn("X-Content-Type-Options", response.headers)
		self.assertEqual("nosniff", response.headers["X-Content-Type-Options"])

	def test4_auth_returns_xss_protection(self):
		req = GET(config.ui_host, config.ui_port, "/auth")
		response, _ = req()

		self.assertIn("X-XSS-Protection", response.headers)
		self.assertEqual("1; block", response.headers["X-XSS-Protection"])

	def test5_auth_returns_frame_options(self):
		req = GET(config.ui_host, config.ui_port, "/auth")
		response, _ = req()

		self.assertIn("X-Frame-Options", response.headers)
		self.assertEqual("SAMEORIGIN", response.headers["X-Frame-Options"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
