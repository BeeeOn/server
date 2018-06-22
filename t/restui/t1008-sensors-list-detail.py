#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
from datetime import datetime
from datetime import timedelta

from rest import GET, POST, PUT, DELETE

class TestSensorsListDetail(unittest.TestCase):
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
			{"id": config.gateway_id, "name": "Testing Gateway", "timezone_id": "Europe/Prague"}
		))
		response, _ = req()

		self.assertEqual(201, response.status)

		req = POST(config.ui_host, config.ui_port, "/gateways")
		req.authorize(self.session)
		req.body(json.dumps(
			{"id": "1334886476281298", "name": "Availability Gateway", "timezone_id": "Europe/Prague"}
		))
		response, _ = req()

		self.assertEqual(201, response.status)

	"""
	Deassign the gateway 1284174504043136 and close the session.
	"""
	def tearDown(self):
		req = DELETE(config.ui_host, config.ui_port, "/gateways/" + "1334886476281298")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

	def assert_sensor(self, sensor, id, name, type, order, has_current = True):
		self.assertEqual(id, sensor["id"])
		self.assertEqual(name, sensor["display_name"])
		self.assertEqual(type, sensor["type_id"])
		self.assertEqual(order, sensor["order"])

		if has_current:
			self.assertTrue("current" in sensor)
		else:
			self.assertFalse("current" in sensor)

	def assert_find_sensor(self, sensors, id, name, type, order, has_current = True):
		for sensor in sensors:
			if sensor["id"] == id:
				self.assert_sensor(sensor, id, name, type, order, has_current)
				return

		self.fail("no such sensor with id " + id)

	"""
	List all available sensors of device 4698890485085053774. There should be
	5 sensors as specifed in the testing devices.xml.
	"""
	def test1_list_sensors_of_device_temperature(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa335d00019f5234e/sensors")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(5, len(result["data"]))

		self.assert_find_sensor(result["data"], "0", "room temperature", "temperature", "0")
		self.assert_find_sensor(result["data"], "1", "outside temperature", "temperature", "1")
		self.assert_find_sensor(result["data"], "2", "room humidity", "humidity", "2")
		self.assert_find_sensor(result["data"], "3", "battery", "battery", "3", False)
		self.assert_find_sensor(result["data"], "4", "signal", "rssi", "4", False)

	"""
	Ask for details about temperature sensor of device 4698890485085053774.
	"""
	def test2_detail_room_temperature(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa335d00019f5234e/sensors/0")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assert_sensor(result["data"], "0", "room temperature", "temperature", "0")
		self.assertEqual("19.500000", result["data"]["current"]["value"])

	"""
	Ask for current value of the temperature sensor of device 4698890485085053774.
	"""
	def test3_current_room_temperature(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa335d00019f5234e/sensors/0/current")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual("19.500000", result["data"]["value"])

	"""
	List all available sensors of device 4931887558509748622. There should be
	9 sensors as specifed in the testing devices.xml.
	"""
	def test4_list_sensors_of_device_unknown(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa371959aad24618e/sensors")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(9, len(result["data"]))

		self.assert_find_sensor(result["data"], "0", "temperature", "temperature", "0", False)
		self.assert_find_sensor(result["data"], "1", "humidity", "humidity", "1", True)
		self.assert_find_sensor(result["data"], "2", "noise", "noise", "2", False)
		self.assert_find_sensor(result["data"], "3", "temperature", "temperature", "3", False)
		self.assert_find_sensor(result["data"], "4", "humidity", "humidity", "4", False)
		self.assert_find_sensor(result["data"], "5", "noise", "noise", "5", False)
		self.assert_find_sensor(result["data"], "6", "temperature", "temperature", "6", False)
		self.assert_find_sensor(result["data"], "7", "humidity", "humidity", "7", False)
		self.assert_find_sensor(result["data"], "8", "noise", "noise", "8", False)

	"""
	Ask for details about temperature sensor of device 4931887558509748622.
	"""
	def test5_detail_unknown_temperature(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa371959aad24618e/sensors/0")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assert_sensor(result["data"], "0", "temperature", "temperature", "0", False)
		self.assertEqual("zone 1", result["data"]["group"])

	"""
	Ask for current value of the temperature sensor of device 4931887558509748622.
	"""
	def test6_current_unknown_temperature(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa371959aad24618e/sensors/0/current")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertTrue("data" in result)
		self.assertIsNone(result["data"])

	"""
	There is no sensor of id 10 for device 4931887558509748622.
	"""
	def test7_detail_non_existing_sensor(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa371959aad24618e/sensors/10")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

	"""
	There is no sensor of id 10 for device 4931887558509748622.
	"""
	def test8_current_non_existing_sensor(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa371959aad24618e/sensors/10/current")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

	"""
	There is no such device 4931887558509748621.
	"""
	def test9_detail_non_existing_device(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0x4471959aad24618f/sensors/10")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

	"""
	There is no such device 4931887558509748621.
	"""
	def testa_list_non_existing_device(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0x4471959aad24618f/sensors")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

	def testb_history_room_temperature_1hour(self):
		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa335d00019f5234e/sensors/0/history" +
				"?range=hour")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(14, len(result["data"]))

		# the same request with explicit timestamps
		start = datetime.now() - timedelta(0, 60 * 60)
		end = datetime.now()

		req = GET(config.ui_host, config.ui_port,
				"/gateways/1284174504043136/devices/0xa335d00019f5234e/sensors/0/history" +
				"?range=%s,%s" % (start.strftime("%s"), end.strftime("%s")))
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(14, len(result["data"]))

	def testc_availability_1hour(self):
		end = datetime(2018, 4, 10, 9)
		start = end - timedelta(0, 60 * 60)

		req = GET(config.ui_host, config.ui_port,
				"/gateways/1334886476281298/devices/0xa3545123f4c823e5/sensors/0/history" +
				"?range=%s,%s&aggregation=freq&interval=300"
				% (start.strftime("%s"), end.strftime("%s")))
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(12, len(result["data"]))

	def testd_availability_1week(self):
		end = datetime(2018, 4, 10, 9)
		start = end - timedelta(7)

		req = GET(config.ui_host, config.ui_port,
				"/gateways/1334886476281298/devices/0xa3545123f4c823e5/sensors/0/history" +
				"?range=%s,%s&aggregation=freq&interval=3600"
				% (start.strftime("%s"), end.strftime("%s")))
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(336, len(result["data"]))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
