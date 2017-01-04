#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
import gc

from rest import POST, PUT, DELETE, GET

class TestFetchUpdateGateway(unittest.TestCase):
	"""
	Prepare a session and a place to associate with a gateway and them.
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

		req = POST(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		req.body('{"name" : "Joe Doe\'s Gateway"}')
		response, content = req()

		self.assertEqual(200, response.status)

		place = json.dumps({"name": "My Second Home"})
		req = POST(config.ui_host, config.ui_port, "/place")
		req.authorize(self.session)
		req.body(place)
		response, content = req()

		self.assertEqual(200, response.status)
		self.second_place = json.loads(content)

	"""
	Destroy the place "My Home" and the session.
	Unassign the gateway.
	"""
	def tearDown(self):
		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.second_place["id"])
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(200, response.status)

		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.place["id"]
				+ "/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.place["id"])
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(200, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(200, response.status)

		#gc.collect()

	"""
	Try to fetch a non-existing gateway.
	"""
	def test0_get_non_existing(self):
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/1464123670904752")

		req.authorize(self.session)
		response, content = req()

		self.assertEqual(403, response.status)
		self.assertEqual(0, len(content))

	"""
	Request an existing gateway.
	"""
	def test1_get_existing(self):
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		gate = json.loads(content)

		self.assertEqual(config.gateway_id, gate["id"])
		self.assertEqual("Joe Doe's Gateway", gate["name"])
		self.assertEqual("0.0.0.0", gate["ip"])
		self.assertEqual("", gate["version"])
		self.assertFalse("last_changed" in gate)

	"""
	Request an existing gateway but from wrong context.
	This tests that the API checks a gateway is assigned
	to the place we are accessing.
	"""
	def test2_get_existing_from_wrong_place(self):
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.second_place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)
		self.assertEqual(0, len(content))

	"""
	Update the gateway with new name and altitude (partial update).
	Altitude should stay a number (UI server shouldn't convert it to
	string).

	After the successful edit revert all changes.
	"""
	def test3_update_partial(self):
		# Test if the values are set as expected
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		gateway = json.loads(content)

		self.assertEqual(config.gateway_id, gateway["id"])
		self.assertEqual("Joe Doe's Gateway", gateway["name"])
		self.assertEqual(0, gateway["altitude"])
		self.assertEqual(0, gateway["longitude"])
		self.assertEqual(0, gateway["latitude"])

		# Do an update of altitude and gateway name
		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		req.body(json.dumps({
			"name" : "Test edited gateway name",
			"altitude" : 100
		}))
		response, content = req()

		self.assertEqual(200, response.status)
		gateway = json.loads(content)

		self.assertEqual(config.gateway_id, gateway["id"])
		self.assertEqual("Test edited gateway name", gateway["name"])
		self.assertEqual(100, gateway["altitude"])
		self.assertEqual(0, gateway["longitude"])
		self.assertEqual(0, gateway["latitude"])
		self.assertEqual("0.0.0.0", gateway["ip"])
		self.assertEqual("", gateway["version"])
		self.assertFalse("last_changed" in gateway)

		# Test if the values were really updated
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		gateway = json.loads(content)

		self.assertEqual(config.gateway_id, gateway["id"])
		self.assertEqual("Test edited gateway name", gateway["name"])
		self.assertEqual(100, gateway["altitude"])
		self.assertEqual(0, gateway["longitude"])
		self.assertEqual(0, gateway["latitude"])
		self.assertEqual("0.0.0.0", gateway["ip"])
		self.assertEqual("", gateway["version"])
		self.assertFalse("last_changed" in gateway)

		# Revert edits on gateway
		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		req.body(json.dumps({
			"name" : "Joe Doe's Gateway",
			"altitude" : 0
		}))
		response, _ = req()

		self.assertEqual(200, response.status)

	"""
	Update the gateway with all available fields + an extra field which should
	be ignored.

	After the successful edit revert all changes.
	"""
	def test4_update_full(self):
		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		req.body(json.dumps({
			"name" : "Test edited gateway name",
			"altitude" : 100,
			"latitude" : 48.0673357,
			"longitude" : 12.8600699,
		}))
		response, content = req()

		self.assertEqual(200, response.status)

		gateway = json.loads(content)

		self.assertEqual(config.gateway_id, gateway["id"])
		self.assertEqual("Test edited gateway name", gateway["name"])
		self.assertEqual(100, gateway["altitude"])
		self.assertEqual(48.0673357, gateway["latitude"])
		self.assertEqual(12.8600699, gateway["longitude"])
		self.assertEqual("0.0.0.0", gateway["ip"])
		self.assertEqual("", gateway["version"])
		self.assertFalse("last_changed" in gateway)

		# Fetch the gateway once again
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		gateway = json.loads(content)

		self.assertEqual(config.gateway_id, gateway["id"])
		self.assertEqual("Test edited gateway name", gateway["name"])
		self.assertEqual(100, gateway["altitude"])
		self.assertEqual(48.0673357, gateway["latitude"])
		self.assertEqual(12.8600699, gateway["longitude"])
		self.assertEqual("0.0.0.0", gateway["ip"])
		self.assertEqual("", gateway["version"])
		self.assertFalse("last_changed" in gateway)

		# Revert edits on gateway
		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		req.body(json.dumps({
			"name" : "Joe Doe's Gateway",
			"altitude" : 0,
			"latitude" : 0,
			"longitude" : 0
		}))
		response, _ = req()

		self.assertEqual(200, response.status)

	"""
	Update the gateway with an extra field which should be ignored.
	"""
	def test5_update_non_existing_field(self):
		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		req.body(json.dumps({
			"extra_field" : "foo"
		}))
		response, content = req()

		self.assertEqual(200, response.status)

		gateway = json.loads(content)

		self.assertNotIn("extra_field", gateway)
		self.assertEqual(config.gateway_id, gateway["id"])
		self.assertEqual("Joe Doe's Gateway", gateway["name"])
		self.assertEqual(0, gateway["altitude"])
		self.assertEqual(0, gateway["longitude"])
		self.assertEqual(0, gateway["latitude"])
		self.assertEqual("0.0.0.0", gateway["ip"])
		self.assertEqual("", gateway["version"])
		self.assertFalse("last_changed" in gateway)

	"""
	Test we cannot update a gateway in place it is not assigned to.
	Check that the gateway is never updated.
	"""
	def test6_update_in_wrong_place(self):
		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.second_place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		req.body(json.dumps({
			"latitude" : 1000
		}))
		response, content = req()

		self.assertEqual(404, response.status)
		self.assertEqual(0, len(content))

		# test on the right place
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		gateway = json.loads(content)

		self.assertEqual(config.gateway_id, gateway["id"])
		self.assertEqual("Joe Doe's Gateway", gateway["name"])
		self.assertEqual(0, gateway["altitude"])
		self.assertEqual(0, gateway["longitude"])
		self.assertEqual(0, gateway["latitude"])
		self.assertEqual("0.0.0.0", gateway["ip"])
		self.assertEqual("", gateway["version"])
		self.assertFalse("last_changed" in gateway)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))

