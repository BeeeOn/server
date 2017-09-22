#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
import os

from rest import POST, DELETE

class TestFCMTokenRegisterUnregister(unittest.TestCase):
	"""
	Create a session for testing.
	"""
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
		response, content = req()

		self.assertEqual(204, response.status)

	@unittest.skipIf("FCM_AVAILABLE" not in os.environ,
			"fcm support is disabled")
	def test_register_unregister_token(self):
		req = POST(config.ui_host, config.ui_port, "/notifications")
		req.authorize(self.session)
		req.body(json.dumps(
			{"id": config.fcm_token, "name": "fcm"}
		))
		response, _ = req()

		self.assertEqual(204, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/notifications");
		req.authorize(self.session)
		req.body(json.dumps(
			{"id": config.fcm_token, "name": "fcm"}
		));
		response, _ = req()

		self.assertEqual(204, response.status)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
