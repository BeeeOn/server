#! /usr/bin/env python3

import config

config.import_libs()

import unittest
import socket
import json
import time

from rest import GET, POST, DELETE

class TestDevicesListDetail(unittest.TestCase):
	""" 
	Create a session for testing and pair a default gateway.
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

	"""
	Unpair the gateway and logout.
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

	"""
	Get all active devices. There should be 3 devices already
	prepared for testing. The humidity devices is expected to
	be available at the time of running this test.
	"""
	def test1_list(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/devices?filter=active")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual(3, len(result["data"]))

		names = ["Temperature", "Humidity", "Multi-sensor"]
		for name in names:
			found = False
			for e in result["data"]:
				if e.get("name") == name:
					found = True
					break

		self.assertTrue(found, "missing: " + name)

		for e in result["data"]:
			self.assertKnownDevice(e)

	"""
	Get detail of the existing device 4698890485085053774 (Temperature).
	"""
	def test2_detail(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/devices/" + "0xa335d00019f5234e")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertKnownDevice(result["data"])

	"""
	Start discovering new devices on gateway. The process is asynchronous and thus we
	should be redirected to the work entity and wait until it finishes (has finished
	timestamp).
	"""
	def test3_discover(self):
		req = POST(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/discovery")
		req.authorize(self.session)
		req.body(json.dumps({"time_limit": 1}))
		response, content = req()

		self.assertEqual(202, response.status)

		data = json.loads(content)
		self.assertEqual("success", data["status"])

		scan_uri = response.getheader("Location")
		self.assertEqual("/gateways/" + config.gateway_id + "/discovery", scan_uri)

		for i in range(10):
			req = GET(config.ui_host, config.ui_port, scan_uri)
			req.authorize(self.session)
			response, content = req()

			self.assertEqual(200, response.status)

			result = json.loads(content)
			self.assertTrue("success", result["status"])

			if result["data"]["state"]["name"] == "finished":
				break

			self.assertTrue("started" in result["data"])
			self.assertTrue("duration" in result["data"])
			self.assertEqual(1, result["data"]["duration"])

			time.sleep(1)

	"""
	Get all active devices via the filter=active.
	"""
	def test4_list_active(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/devices?filter=active")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual(3, len(result["data"]))

	"""
	Get all inactive devices via the filter=inactive.
	"""
	def test5_list_inactive(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/devices?filter=inactive")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual(2, len(result["data"]))

	"""
	Ask for a device with invalid EUID. Parsing should fail on invalid input.
	"""
	def test6_detail_invalid_euid(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/devices/" + "a1231r30430")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(400, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])
		self.assertEqual("could not process input data", result["message"])

	"""
	Ask for a device with too big EUID (oveflows 64-bit unsigned integer).
	"""
	def test7_detail_big_euid(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/devices/" + "1208925819614629174706175")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(400, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])
		self.assertEqual("could not process input data", result["message"])

	"""
	Get device 0xa3779fbec542e18b that has unsupported type defined (missing in device.xml).
	"""
	def test8_unsupported_device(self):
		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/devices/0xa3779fbec542e18b")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual("0xa3779fbec542e18b", result["data"]["id"])
		self.assertIn("type", result["data"])
		self.assertNotIn("name", result["data"]["type"])

	def assertDeviceIsComplete(self, e):
		self.assertIsNotNone(e.get("id", None))
		self.assertIsNotNone(e.get("location_id", None))
		self.assertIsNotNone(e.get("name", None))
		self.assertIsNotNone(e.get("type", None))
		self.assertIsNotNone(e.get("first_seen", None))
		self.assertIsNotNone(e.get("last_seen", None))
		self.assertIsNotNone(e.get("active_since", None))
		self.assertIsNotNone(e.get("available", None))

	def assertKnownDevice(self, e):
		id = e.get("id")
		self.assertDeviceIsComplete(e)

		if id == "0xa335d00019f5234e":
			self.assertEqual("Temperature", e.get("name"))
			self.assertEqual(1, e.get("available"))
			self.assertIsNone(e.get("refresh_time", None))
		elif id == "0xa32d27aa5e94ecfd":
			self.assertEqual("Multi-sensor", e.get("name"))
			self.assertEqual(0, e.get("available"))
			self.assertIsNotNone(e.get("refresh_time", None))
		elif id == "0xa37e0f7f0302324d":
			self.assertEqual("Humidity", e.get("name"))
			self.assertEqual(1, e.get("available"))
			self.assertIsNotNone(e.get("refresh_time", None))
		elif id == "0xa371959aad24618e":
			self.assertEqual("Unknown", e.get("name"))
			self.assertEqual(1, e.get("available"))
			self.assertIsNotNone(e.get("refresh_time", None))
		else:
			self.assertTrue(False, "unknown device: %s, %s, %s", (e.tag, e.attrib, e.text))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
