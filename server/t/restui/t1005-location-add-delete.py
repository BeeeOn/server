#! /usr/bin/env python3

import config

config.import_libs()

import unittest
import json

from rest import GET, POST, DELETE, PUT

class TestLocationAddDelete(unittest.TestCase):
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
	Add location to the prepared gateway and then delete it.
	"""
	def test1_add_and_delete(self):
		req = POST(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/locations")
		req.authorize(self.session)
		req.body(json.dumps({
			"name": "New location",
			"type": 0
		}))
		response, content = req()

		self.assertEqual(201, response.status)

		req = DELETE(config.ui_host, config.ui_port, response.headers["Location"])

		req.authorize(self.session)
		response, content = req()
		self.assertEqual(204, response.status)

if __name__ == '__main__':
	import sys
	import taprunner

	unittest.main(testRunner=taprunner.TAPTestRunner(stream=sys.stdout))
