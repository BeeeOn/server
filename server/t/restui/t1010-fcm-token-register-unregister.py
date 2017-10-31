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

	@unittest.skipIf("FCM_SENDER_ID" not in os.environ,
			"fcm support is disabled")
	def test1_list_services(self):
		req = GET(config.ui_host, config.ui_port, "/notifications")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(1, len(result["data"]))
		self.assertEqual("fcm", result["data"][0]["name"])
		self.assertEqual(os.environ["FCM_SENDER_ID"], result["data"][0]["id"])

	@unittest.skipIf("FCM_SENDER_ID" not in os.environ,
			"fcm support is disabled")
	def test2_register_unregister_token(self):
		req = POST(config.ui_host, config.ui_port, "/notifications/fcm")
		req.authorize(self.session)
		req.body(json.dumps(
			{"id": config.fcm_token}
		))
		response, _ = req()

		self.assertEqual(204, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/notifications/fcm/" + config.fcm_token);
		req.authorize(self.session)

		self.assertEqual(204, response.status)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
