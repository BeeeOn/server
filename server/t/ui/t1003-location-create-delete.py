#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import POST, PUT, DELETE, GET

class TestLocation(unittest.TestCase):
	"""
	Prepare a session.
	"""
	def setUp(self):
		print("# setup")

		req = POST(config.ui_host, config.ui_port, "/auth")
		req.body(config.PERMIT_LOGIN)
		response, content = req()

		self.assertEqual(200, response.status)
		self.session = content
		print("# got session: " + self.session)

		place = json.dumps({"name": "My Home"})
		req = POST(config.ui_host, config.ui_port, "/place")
		req.authorize(self.session)
		req.body(place)
		response, content = req()

		self.assertEqual(200, response.status)
		self.place = json.loads(content)
		print("# got place: " + self.place["id"])

	"""
	Destroy the session.
	"""
	def tearDown(self):
		print("# teardown")

		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.place["id"])
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(200, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(200, response.status)

	"""
	Test create location within the prepared place 'My Home',
	delete it and test it does not exist anymore.
	"""
	def test1_create_delete(self):
		req = POST(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] + "/locations")

		location = json.dumps({
			"name" : "Test Location"
		})
		req.authorize(self.session)
		req.body(location)
		response, content = req()

		self.assertEqual(200, response.status)
		location = json.loads(content)
		self.assertEqual("Test Location", location["name"])
		self.assertEqual(self.place["id"], location["place_id"])
		self.assertIn("id", location)

		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" + location["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("Test Location", result["name"])
		self.assertEqual(self.place["id"], result["place_id"])
		self.assertEqual(location["id"], result["id"])

		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" + location["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(403, response.status)
		self.assertEqual(0, len(content))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))

