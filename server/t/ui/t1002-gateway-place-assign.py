#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import POST, PUT, DELETE, GET

class TestGatewayPlaceAssign(unittest.TestCase):
	"""
	Prepare a session and a place "My Home".
	"""
	def setUp(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		req.body(config.PERMIT_LOGIN)
		response, content = req()

		self.assertEqual(200, response.status)
		self.session = content

		place = json.dumps({"name": "My Home"})
		req = POST(config.ui_host, config.ui_port, "/place")
		req.authorize(self.session)
		req.body(place)
		response, content = req()

		self.assertEqual(200, response.status)
		self.place = json.loads(content)

		place = json.dumps({"name": "My Second Home"})
		req = POST(config.ui_host, config.ui_port, "/place")
		req.authorize(self.session)
		req.body(place)
		response, content = req()

		self.assertEqual(200, response.status)
		self.second_place = json.loads(content)

	"""
	Destroy the place "My Home" and the session.
	"""
	def tearDown(self):
		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.second_place["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.place["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

	"""
	Test assign and unassign of well-known virtual testing gateway
	1780053541714013 to the prepared place 'My Home'.
	"""
	def test1_assign_unassign(self):
		req = POST(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.tmp_gateway_id)

		gateway = json.dumps({
			"name" : "Test Gateway Assigned"
		})
		req.body(gateway)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		gateway = json.loads(content)
		self.assertEqual("Test Gateway Assigned", gateway["name"])
		self.assertEqual(self.place["id"], gateway["place_id"])

		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.tmp_gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		gateway = json.loads(content)
		self.assertEqual(config.tmp_gateway_id, gateway["id"])

		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.tmp_gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(403, response.status)
		self.assertEqual(0, len(content))

	"""
	Test cannot assign an already assigned place. To move a gateway
	to a different place, it is necessary to unassign it first.
	"""
	def test2_cannot_steal_gateway(self):
		req = POST(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.tmp_gateway_id)
		gateway = json.dumps({
			"name" : "Test Gateway Assigned"
		})
		req.body(gateway)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		gateway = json.loads(content)
		self.assertEqual("Test Gateway Assigned", gateway["name"])
		self.assertEqual(self.place["id"], gateway["place_id"])

		req = POST(config.ui_host, config.ui_port,
				"/place/" + self.second_place["id"] +
				"/gateways/" + config.tmp_gateway_id)

		gateway = json.dumps({
			"name" : "Test Gateway Stolen"
		})
		req.body(gateway)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)
		self.assertEqual(0, len(content))

		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.tmp_gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		notStolen = json.loads(content)
		self.assertEqual("Test Gateway Assigned", notStolen["name"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
