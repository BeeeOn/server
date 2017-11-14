#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
import os

from rest import GET, POST, DELETE

class TestAttack(unittest.TestCase):
	def setUp(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		req.body(config.PERMIT_LOGIN)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.session = result["data"]["id"]

	def tearDown(self):
		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(204, response.status)

	"""
	Send malformed UTF-8 data in session ID.
	"""
	def test1_exploit_invalid_session(self):
		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize("\xc0\x8d\xc0\x8a\xc0\x8d\xc0\x8a{\"attack\": \"payload\"}")
		response, content = req.perform()

		self.assertEqual(401, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])
		self.assertEqual(401, result["code"])

	"""
	Send URI with injected '..' encoded as invalid UTF-8 characters.
	Expect that server would avoid processing of them. Currently, the
	placeholder is '?' so it is dropped by URI parser and the resulted
	URI for the server is /gateways. There are no gateways for the user we
	are using.
	"""
	def test2_exploit_get_uri(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/%c0%ae%c0%ae")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(200, result["code"])
		self.assertEqual(0, len(result["data"]))

	"""
	Test XSS is prevented when creating and deleting place. This is just
	one of potential inputs however it shows that the applicated sanitization
	works as expected. The Sanitize::common() is applicated here.
	"""
	def test3_simple_xss_attempt(self):
		req = POST(config.ui_host, config.ui_port, "/gateways")
		req.authorize(self.session)
		req.body(json.dumps(
			{"id": config.gateway_id, "name": "My <script>attack()</script>", "timezone_id": "Europe/Prage"}
		))
		response, content = req()

		self.assertEqual(400, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])
		self.assertEqual(400, result["code"])
		self.assertEqual("invalid input data", result["message"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
