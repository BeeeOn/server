#! /usr/bin/env python3

import config

config.import_libs()

import unittest
import json

from rest import GET, POST, DELETE, PUT

class TestLocationGetUpdate(unittest.TestCase):
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
		req.body(json.dumps(
			{"id": config.gateway_id, "name": "Testing Gateway"}
		))
		response, _ = req()

		if response.status != 403:
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
	Update existing location
	"""
	def test2_get_and_update(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/locations")
		req.authorize(self.session)
		response, content = req()

		result = json.loads(content)

		self.assertEqual(200, response.status)

		location = result["data"][0]

		req = PUT(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id + "/locations/" + location["id"])

		req.authorize(self.session)
		req.body(json.dumps({
			"name": "New location2"
		}))

		response, content = req()
		result = json.loads(content)

		self.assertEqual(200, response.status)
		self.assertEqual("New location2", result["data"]["name"])


if __name__ == '__main__':
	import sys
	import taprunner

	unittest.main(testRunner=taprunner.TAPTestRunner(stream=sys.stdout))
