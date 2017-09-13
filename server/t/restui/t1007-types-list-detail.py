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

	def assure_range(self, id, name, min, max, step):
		req = GET(config.ui_host, config.ui_port, "/types/" + id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		type = result["data"]
		self.assertEqual(name, type["name"])
		self.assertTrue("range" in type)

		if min is not None:
			self.assertTrue("min" in type["range"])
			self.assertEqual(min, type["range"]["min"])
		else:
			self.assertFalse("min" in type["range"])

		if max is not None:
			self.assertTrue("max" in type["range"])
			self.assertEqual(max, type["range"]["max"])
		else:
			self.assertFalse("max" in type["range"])

		if step is not None:
			self.assertTrue("step" in type["range"])
			self.assertEqual(step, type["range"]["step"])
		else:
			self.assertFalse("step" in type["range"])

	def test4_check_types_with_ranges(self):
		self.assure_range("2", "battery", 0, 100, 1)
		self.assure_range("4", "brightness", 0, 100, 1)
		self.assure_range("5", "CO2", 0, 1000000, 1)
		self.assure_range("8", "humidity", 0, 100, 1)
		self.assure_range("9", "luminance", 0, 1000000, 1)
		self.assure_range("11", "noise", 0, 200, 1)
		self.assure_range("14", "performance", 0, 100, 1)
		self.assure_range("15", "pressure", 800, 1100, 1)
		self.assure_range("16", "signal", 0, 100, 1)
		self.assure_range("19", "temperature", -273.15, 200, 0.01)
		self.assure_range("20", "UV", 0, 11, 0.1)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
