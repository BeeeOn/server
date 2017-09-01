#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import GET, POST, PUT, DELETE

class TestTypesListDetail(unittest.TestCase):
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
	List all available types.
	"""
	def test1_list_all(self):
		req = GET(config.ui_host, config.ui_port, "/types")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(20, len(result["data"]))

	def test2_detail_of_non_existing(self):
		req = GET(config.ui_host, config.ui_port, "/types/12904232")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

		result = json.loads(content)
		self.assertEqual("error", result["status"])
		self.assertEqual("requested resource does not exist", result["message"])

	def test3_detail_of_battery(self):
		req = GET(config.ui_host, config.ui_port, "/types/2")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual("battery", result["data"]["name"])
		self.assertEqual("%", result["data"]["unit"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
