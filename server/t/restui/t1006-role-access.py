#! /usr/bin/env python3

import config

config.import_libs()

import unittest
import json

from rest import GET, POST, DELETE, PUT


class TestRole(unittest.TestCase):
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

		req = POST(config.ui_host, config.ui_port, "/gateways")
		req.authorize(self.session)
		req.body(json.dumps({
			"id": config.gateway_id, "name": "Testing Gateway", "timezone_id": "Europe/Prague"
		}))
		response, _ = req()

		self.assertEqual(201, response.status)

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
	Access my current role in the given gateway.
	"""
	def test1_access_myself(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/roles/current")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(303, response.status)

		req = GET(config.ui_host, config.ui_port, response.headers["Location"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(200, result["code"])

		self.assertEqual("joe.doe@example.org", result["data"]["identity_email"])
		self.assertEqual("admin", result["data"]["access_level"])
		self.assertEqual("Joe", result["data"]["user"]["first_name"])
		self.assertEqual("Doe", result["data"]["user"]["last_name"])

if __name__ == '__main__':
	import sys
	import taprunner

	unittest.main(testRunner=taprunner.TAPTestRunner(stream=sys.stdout))
