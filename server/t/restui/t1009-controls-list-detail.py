#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
from datetime import datetime
from datetime import timedelta

from rest import GET, POST, PUT, DELETE

class TestControlsListDetail(unittest.TestCase):
	"""
	Create a session for testing and assign the gateway 1284174504043136.
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
		req.body(json.dumps(
			{"id": config.gateway_id, "name": "Testing Gateway"}
		))
		response, _ = req()

		self.assertEqual(201, response.status)

	"""
	Deassign the gateway 1284174504043136 and close the session.
	"""
	def tearDown(self):
		req = DELETE(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

	def assert_control(self, control, id, name, type, order, has_current = True):
		self.assertEqual(id, control["id"])
		self.assertEqual(name, control["display_name"])
		self.assertEqual(type, control["type_id"])
		self.assertEqual(order, control["order"])

		if has_current:
			self.assertTrue("current" in control)
		else:
			self.assertFalse("current" in control)

	def assert_find_control(self, controls, id, name, type, order, has_current = True):
		for control in controls:
			if control["id"] == id:
				self.assert_control(control, id, name, type, order, has_current)
				return

		self.fail("no such control with id " + id)

	"""
	List all available controls of device 4931887558509748622. There should be
	1 control as specifed in the testing devices.xml.
	"""
	def test1_list_controls_of_device_unknown(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0x4471959aad24618e/controls")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		print(content)
		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(1, len(result["data"]))

		self.assert_find_control(result["data"], "9", "brightness", "4", "9", False)

	"""
	Ask for details about brightness control of device 4931887558509748622.
	"""
	def test2_detail_unknown_brightness(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0x4471959aad24618e/controls/9")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assert_control(result["data"], "9", "brightness", "4", "9", False)

	"""
	There is no control of id 1000 for device 4931887558509748622.
	"""
	def test3_detail_non_existing_control(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0x4471959aad24618e/sensors/1000")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

	"""
	There is no sensor of id 1000 for device 4931887558509748622.
	"""
	def test4_current_non_existing_sensor(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0x4471959aad24618e/controls/1000/current")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

	"""
	There is no such device 4931887558509748621.
	"""
	def test5_detail_non_existing_device(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0x4471959aad24618f/controls/9")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

	"""
	There is no such device 4931887558509748621.
	"""
	def test6_list_non_existing_device(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0x4471959aad24618f/controls")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
