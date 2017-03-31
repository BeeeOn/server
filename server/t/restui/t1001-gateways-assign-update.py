#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import GET, POST, PUT, DELETE

class TestGatewayAssignUpdate(unittest.TestCase):
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

	"""
	Assign testing gateway 1284174504043136 and check its contents.
	Unassign the gateway and check it became inaccessible again.
	"""
	def test1_assign_unassign(self):
		# assign a prepared gateway
		req = POST(config.ui_host, config.ui_port, "/gateways")
		req.authorize(self.session)
		req.body(json.dumps({
			"id": config.gateway_id,
			"name": "My Home"
		}))

		response, content = req()
		self.assertEqual(201, response.status)
		self.assertEqual("/gateways/" + config.gateway_id,
				response.getheader("Location"))

		# check the gateway's contents
		resultLink = response.getheader("Location")
		req = GET(config.ui_host, config.ui_port, resultLink)
		req.authorize(self.session)

		response, content = req()
		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual("My Home", result["data"]["name"])
		self.assertEqual(config.gateway_id, result["data"]["id"])

		# unassign the gateway
		req = DELETE(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

		# test the gateway is inaccessible
		req = GET(config.ui_host, config.ui_port, resultLink)
		req.authorize(self.session)

		response, content = req()
		self.assertEqual(403, response.status)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
