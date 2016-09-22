#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import POST, DELETE, GET

class TestPlace(unittest.TestCase):
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

	"""
	Destroy the session.
	"""
	def tearDown(self):
		print("# teardown")
		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(200, response.status)

	"""
	Create a place, delete it and test it does not exist anymore.
	"""
	def test1_create_delete(self):
		place = json.dumps({"name": "My Home"})
		req = POST(config.ui_host, config.ui_port, "/place")
		req.authorize(self.session)
		req.body(place)
		response, content = req()

		self.assertEqual(200, response.status)
		data = json.loads(content)
		self.assertEqual("My Home", data["name"])
		self.assertIn("id", data)

		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + data["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("My Home", result["name"])
		self.assertIn("id", result)

		req = GET(config.ui_host, config.ui_port,
				"/place/" + data["id"])
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(404, response.status)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
