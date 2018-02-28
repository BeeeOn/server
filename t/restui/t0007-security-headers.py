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

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
