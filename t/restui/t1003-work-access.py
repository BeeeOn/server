#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import json

from rest import POST, PUT, GET, DELETE

class TestWorkAccess(unittest.TestCase):
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
	Reading any non-existing work leads to 404.
	"""
	def test1_get_non_existing(self):
		req = GET(config.ui_host, config.ui_port, "/works/4891a1ec-2939-49f8-9207-4ea9f0499934")
		req.authorize(self.session)

		response, content = req()
		result = json.loads(content)
		self.assertEqual(404, response.status)
		self.assertEqual("error", result["status"])

	"""
	Canceling any non-existing work leads to 404.
	"""
	def test2_cancel_non_existing(self):
		req = PUT(config.ui_host, config.ui_port, "/works/4891a1ec-2939-49f8-9207-4ea9f0499934")
		req.authorize(self.session)
		req.body(json.dumps({
			"status": "canceled"
		}))

		response, content = req()
		result = json.loads(content)
		self.assertEqual(404, response.status)
		self.assertEqual("error", result["status"])

	"""
	Remove any non-existing work leads to 404.
	"""
	def test3_remove_non_existing(self):
		req = DELETE(config.ui_host, config.ui_port, "/works/4891a1ec-2939-49f8-9207-4ea9f0499934")
		req.authorize(self.session)

		response, content = req()
		result = json.loads(content)
		self.assertEqual(404, response.status)
		self.assertEqual("error", result["status"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
