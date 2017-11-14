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
	Cannot change own access level to guest instead of user.
	"""
	def test1_cannot_downgrade_myself(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/roles/current")
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(303, response.status)

		req = GET(config.ui_host, config.ui_port, response.headers["Location"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(200, result["code"])

		req = PUT(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/roles/" + result["data"]["id"])
		req.authorize(self.session)
		req.body(json.dumps({
			"access_level": "guest"
		}))
		response, content = req()
		self.assertEqual(403, response.status)

	"""
	Cannot reapply admin access level.
	This is a non-sense operation, just make sure how does it behave.
	"""
	def test2_cannot_update_myself_atall(self):
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

		result = json.loads(content)

		req = PUT(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/roles/" + result["data"]["id"])
		req.authorize(self.session)
		req.body(json.dumps({
			"access_level": "admin"
		}))
		response, content = req()
		self.assertEqual(403, response.status)

if __name__ == '__main__':
	import sys
	import taprunner

	unittest.main(testRunner=taprunner.TAPTestRunner(stream=sys.stdout))
