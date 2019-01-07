#! /usr/bin/env python3

import config

config.import_libs()

import unittest
import socket
import json

from rest import GET, POST, PUT, DELETE

class TestDevicesProperties(unittest.TestCase):
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

	def test1_list_properties_having_firmware(self):
		req = GET(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual(1, len(result["data"]))
		self.assertEqual("firmware", result["data"][0]["key"])
		self.assertEqual("Firmware", result["data"][0]["display_name"])
		self.assertEqual("v1.0-6453", result["data"][0]["value"])
		self.assertTrue(result["data"][0]["read-only"])
		self.assertNotIn("write-only", result["data"][0])

	def test2_fail_on_create_read_only_firmware(self):
		req = POST(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/firmware")
		req.authorize(self.session)
		req.body(json.dumps({"value": "super firmware"}))
		response, content = req()

		self.assertEqual(405, response.status)
		result = json.loads(content)
		self.assertEqual("Method Not Allowed", result["status"])

	def test3_fail_on_update_read_only_firmware(self):
		req = PUT(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/firmware")
		req.authorize(self.session)
		req.body(json.dumps({"value": "super firmware"}))
		response, content = req()

		self.assertEqual(405, response.status)
		result = json.loads(content)
		self.assertEqual("Method Not Allowed", result["status"])

	def test4_fail_on_delete_read_only_firmware(self):
		req = DELETE(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/firmware")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(405, response.status)
		result = json.loads(content)
		self.assertEqual("Method Not Allowed", result["status"])

	def test5_get_nonexisting_key(self):
		req = GET(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/sssss")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(400, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])

	def test6_create_get_remove_ip_address(self):
		req = GET(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/ip-address")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])

		req = POST(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/ip-address")
		req.authorize(self.session)
		req.body(json.dumps({"value": "192.168.0.1"}))
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		req = GET(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/ip-address")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual("ip-address", result["data"]["key"])
		self.assertEqual("IP address", result["data"]["display_name"])
		self.assertEqual("192.168.0.1", result["data"]["value"])
		self.assertNotIn("read-only", result["data"])
		self.assertNotIn("write-only", result["data"])

		req = DELETE(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/ip-address")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

	def test7_create_get_remove_password(self):
		req = GET(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/password")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])

		req = POST(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/password")
		req.authorize(self.session)
		req.body(json.dumps({"value": "some-password"}))
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		req = GET(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/password")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual("password", result["data"]["key"])
		self.assertEqual("Password", result["data"]["display_name"])
		self.assertIsNone(result["data"]["value"])
		self.assertTrue(result["data"]["write-only"])
		self.assertNotIn("read-only", result["data"])

		req = DELETE(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/password")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

	def test8_create_twice_password(self):
		req = GET(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/password")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])

		req = POST(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/password")
		req.authorize(self.session)
		req.body(json.dumps({"value": "some-password"}))
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		req = POST(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/password")
		req.authorize(self.session)
		req.body(json.dumps({"value": "some-password"}))
		response, content = req()

		self.assertEqual(409, response.status)
		result = json.loads(content)
		self.assertEqual("Conflict", result["status"])

		req = DELETE(config.ui_host, config.ui_port,
			"/gateways/" + config.gateway_id
			+ "/devices/0xa335d00019f5234e/properties/password")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

	def test9_create_properties(self):
		properties = {
			"ip-address": "10.0.0.1",
			"password": "test-password"
		}

		for key, value in properties.items():
			req = POST(config.ui_host, config.ui_port,
				"/gateways/" + config.gateway_id
				+ "/devices/0xa371959aad24618e/properties/" + key)
			req.authorize(self.session)
			req.body(json.dumps({"value": "10.0.0.1"}))
			response, content = req()

			self.assertEqual(200, response.status)

			req = DELETE(config.ui_host, config.ui_port,
				"/gateways/" + config.gateway_id
				+ "/devices/0xa371959aad24618e/properties/" + key)
			req.authorize(self.session)
			response, content = req()

			self.assertEqual(204, response.status)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
