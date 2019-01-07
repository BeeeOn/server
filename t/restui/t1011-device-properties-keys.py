#! /usr/bin/env python3

import config

config.import_libs()

import unittest
import socket
import json

from rest import GET, POST, DELETE

class TestDevicesPropertiesKeys(unittest.TestCase):
	""" 
	Create a session for testing and pair a default gateway.
	"""
	def setUp(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		req.body(config.PERMIT_LOGIN)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.session = result["data"]["id"]

		req = POST(config.ui_host, config.ui_port, "/gateways")
		req.authorize(self.session)
		req.body(json.dumps(
			{"id": config.gateway_id, "name": "Testing Gateway", "timezone_id": "Europe/Prague"}
		))
		response, _ = req()

		self.assertEqual(201, response.status)

	"""
	Unpair the gateway and logout.
	"""
	def tearDown(self):
		req = DELETE(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

	"""
	Get all available device property keys.
	"""
	def test1_keys(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/devices/properties/keys")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual(3, len(result["data"]))

		self.assertEqual("ip-address", result["data"][0]["key"]);
		self.assertEqual("IP address", result["data"][0]["display_name"]);
		self.assertNotIn("read-only", result["data"][0]);
		self.assertNotIn("write-only", result["data"][0]);

		self.assertEqual("password", result["data"][1]["key"]);
		self.assertEqual("Password", result["data"][1]["display_name"]);
		self.assertTrue(result["data"][1]["write-only"]);

		self.assertEqual("firmware", result["data"][2]["key"]);
		self.assertEqual("Firmware", result["data"][2]["display_name"]);
		self.assertTrue(result["data"][2]["read-only"]);

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
