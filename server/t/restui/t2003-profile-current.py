#! /usr/bin/env python3

import config

config.import_libs()

import unittest
import json

from rest import GET, POST, DELETE

class TestProfileCurrent(unittest.TestCase):
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
	Get current profile. Current profile is Joe Doe with verified identity
	prepared:2e0a2555-b5ae-42aa-9fd1-1923c5863a6a.
	"""
	def test1_current(self):
		req = GET(config.ui_host, config.ui_port, "/profiles/current")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(303, response.status)

		req = GET(config.ui_host, config.ui_port, response.headers["Location"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual("joe.doe@example.org", result["data"]["email"])
		self.assertEqual("Joe", result["data"]["first_name"])
		self.assertEqual("Doe", result["data"]["last_name"])
		self.assertEqual("google", result["data"]["provider"])

	"""
	Try to obtain John Smith's identity. We have no permission for this.
	"""
	def test2_inaccessible(self):
		req = GET(config.ui_host, config.ui_port, "/profiles/ab9996fa-5252-4df5-a3f6-1d7efde7a021")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(403, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])
		self.assertEqual("not enough permission to access the resource", result["message"])

	def test3_list(self):
		req = GET(config.ui_host, config.ui_port, "/profiles")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual(2, len(result["data"]))

		# dynamic ID generated on login, we have to as /profiles/current to obtain the ID
		self.assertIsNotNone(result["data"][0]["id"])
		self.assertEqual("joe.doe@example.org", result["data"][0]["email"])
		self.assertEqual("google", result["data"][0]["provider"])

		self.assertEqual("2e0a2555-b5ae-42aa-9fd1-1923c5863a6a", result["data"][1]["id"])
		self.assertEqual("joe.doe@example.org", result["data"][1]["email"])
		self.assertEqual("prepared", result["data"][1]["provider"])

if __name__ == '__main__':
	import sys
	import taprunner

	unittest.main(testRunner=taprunner.TAPTestRunner(stream=sys.stdout))
