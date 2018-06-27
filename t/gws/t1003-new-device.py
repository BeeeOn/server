#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json
import uuid

from gws import assureIsClosed, assureNotClosed, registerGateway

class TestNewDevice(unittest.TestCase):
	def setUp(self):
		self.ws = websocket.WebSocket()
		self.ws.connect(config.gws_ws_uri)

		registerGateway(self, self.ws, config.gateway_id)

	def tearDown(self):
		self.ws.close()

	"""
	Register device with a valid type. It should be accepted without any issue.
	"""
	def test1_new_device_success(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_request",
			"id" : id,
			"device_id" : "0xa123123412341234",
			"vendor" : "BeeeOn",
			"product_name" : "Internal Pressure v1.0",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "pressure",
					"attributes" : [
						{"attribute" : "inner"}
					]
				}
			]
		})

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("generic_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(1, msg["status"])
		assureNotClosed(self, self.ws)

	"""
	Register device that does not match to any types known on the
	server. The server must fail to register this devices and provide
	an appropriate response.
	"""
	def test2_new_device_fail_invalid_product(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_request",
			"id" : id,
			"device_id" : "0xa123123412349999",
			"vendor" : "Good Company",
			"product_name" : "Nice Product",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "pressure",
					"attributes" : [
						{"attribute" : "inner"}
					]
				}
			]
		})

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("generic_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(2, msg["status"])
		assureNotClosed(self, self.ws)

	"""
	Server must deny accepting of device with an invalid device ID.
	The type of the device would be valid and acceptable otherwise.
	"""
	def test3_new_device_disconnect_invalid_id(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_request",
			"id" : id,
			"device_id" : "123",
			"vendor" : "BeeeOn",
			"product_name" : "Internal Pressure v1.0",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "pressure",
					"attributes" : [
						{"attribute" : "inner"}
					]
				}
			]
		})

		self.ws.send(msg)
		assureIsClosed(self, self.ws)

	"""
	Register device that module type 'humidity' does not match with known module
	'pressure' for that device type on the server. The server must fail
	to register this devices and provide an appropriate response.
	"""
	def test4_new_device_fail_invalid_module(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_request",
			"id" : id,
			"device_id" : "0xa123123412341111",
			"vendor" : "BeeeOn",
			"product_name" : "Internal Pressure v1.0",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "humidity",
					"attributes" : [
						{"attribute" : "inner"}
					]
				}
			]
		})

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("generic_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(2, msg["status"])
		assureNotClosed(self, self.ws)

	"""
	Register device with 2 modules, but this device type has just one module
	'pressure' known on the server. The server must fail
	to register this devices and provide an appropriate response.
	"""
	def test5_new_device_fail_too_many_modules(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_request",
			"id" : id,
			"device_id" : "0xa123123412342222",
			"vendor" : "BeeeOn",
			"product_name" : "Internal Pressure v1.0",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "pressure",
					"attributes" : [
						{"attribute" : "inner"}
					]
				},
				{
					"type" : "humidity",
					"attributes" : []
				}
			]
		})

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("generic_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(2, msg["status"])
		assureNotClosed(self, self.ws)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
