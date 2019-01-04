#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json
import uuid

from gws import assureIsClosed, assureNotClosed, registerGateway, ZMQConnection

class TestNewDevice(unittest.TestCase):
	def setUp(self):
		self.zmq = ZMQConnection(config.gws_zmq_endpoint)
		self.zmq.accept(lambda m: "gateway_id" in m)
		self.zmq.open()

		self.ws = websocket.WebSocket()
		self.ws.connect(config.gws_ws_uri)

		registerGateway(self, self.ws, config.gateway_id)

		event = self.zmq.pop_data()
		self.assertEqual("on-connected", event["event"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

	def tearDown(self):
		self.ws.close()

		try:
			event = self.zmq.pop_data()
			self.assertEqual("on-disconnected", event["event"])
			self.assertEqual(config.gateway_id, event["gateway_id"])
		finally:
			self.zmq.close()

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
		response = json.loads(self.ws.recv())

		self.assertEqual("generic_response", response["message_type"])
		self.assertEqual(id, response["id"])
		self.assertEqual(1, response["status"])
		assureNotClosed(self, self.ws)

		event = self.zmq.pop_data()
		self.assertEqual("on-new-device", event["event"])
		self.assertEqual("0xa123123412341234", event["device_id"])
		self.assertEqual("Internal Pressure v1.0", event["device_name"])
		self.assertEqual(1, len(event["modules"]))
		self.assertEqual("pressure", event["modules"][0]["type"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

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
		response = json.loads(self.ws.recv())

		self.assertEqual("generic_response", response["message_type"])
		self.assertEqual(id, response["id"])
		self.assertEqual(2, response["status"])
		assureNotClosed(self, self.ws)

		event = self.zmq.pop_data()
		self.assertEqual("on-refused-new-device", event["event"])
		self.assertEqual("0xa123123412349999", event["device_id"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

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
		response = json.loads(self.ws.recv())

		self.assertEqual("generic_response", response["message_type"])
		self.assertEqual(id, response["id"])
		self.assertEqual(2, response["status"])
		assureNotClosed(self, self.ws)

		event = self.zmq.pop_data()
		self.assertEqual("on-refused-new-device", event["event"])
		self.assertEqual("0xa123123412341111", event["device_id"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

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
		response = json.loads(self.ws.recv())

		self.assertEqual("generic_response", response["message_type"])
		self.assertEqual(id, response["id"])
		self.assertEqual(2, response["status"])
		assureNotClosed(self, self.ws)

		event = self.zmq.pop_data()
		self.assertEqual("on-refused-new-device", event["event"])
		self.assertEqual("0xa123123412342222", event["device_id"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

	"""
	Test that a second new_device_request for the same device where its product_name
	is not important for type recognition is used as an update and its name is
	updated accordingly.
	"""
	def test6_new_device_and_update(self):
		id = str(uuid.uuid4())

		msg1 = json.dumps({
			"message_type" : "new_device_request",
			"id" : id,
			"device_id" : "0xa123123412343333",
			"vendor" : "Availability",
			"product_name" : "",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "availability",
					"attributes" : []
				}
			]
		})

		self.ws.send(msg1)
		response = json.loads(self.ws.recv())

		self.assertEqual("generic_response", response["message_type"])
		self.assertEqual(id, response["id"])
		self.assertEqual(1, response["status"])
		assureNotClosed(self, self.ws)

		notification = self.zmq.pop_data()
		self.assertEqual("on-new-device", notification["event"])
		self.assertEqual("0xa123123412343333", notification["device_id"])
		self.assertNotIn("device_name", notification)
		self.assertEqual(1, len(notification["modules"]))
		self.assertEqual("availability", notification["modules"][0]["type"])
		self.assertEqual(config.gateway_id, notification["gateway_id"])

		msg2 = json.dumps({
			"message_type" : "new_device_request",
			"id" : id,
			"device_id" : "0xa123123412343333",
			"vendor" : "Availability",
			"name": "Super device",
			"product_name" : "",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "availability",
					"attributes" : []
				}
			]
		})

		self.ws.send(msg2)
		response = json.loads(self.ws.recv())

		self.assertEqual("generic_response", response["message_type"])
		self.assertEqual(id, response["id"])
		self.assertEqual(1, response["status"])
		assureNotClosed(self, self.ws)

		notification = self.zmq.pop_data()
		self.assertEqual("on-new-device", notification["event"])
		self.assertEqual("0xa123123412343333", notification["device_id"])
		self.assertEqual("Super device", notification["device_name"])
		self.assertEqual(1, len(notification["modules"]))
		self.assertEqual("availability", notification["modules"][0]["type"])
		self.assertEqual(config.gateway_id, notification["gateway_id"])

	"""
	Register new device with unknown modules. Such device should
	be registered successfully. The modules are resolved by server
	and reported via ZeroMQ events.
	"""
	def test7_new_device_with_unknown_modules(self):
		id = str(uuid.uuid4())

		msg1 = json.dumps({
			"message_type" : "new_device_request",
			"id" : id,
			"device_id" : "0xa123123412355431",
			"vendor" : "Unknown sensors producer",
			"product_name" : "Device with unknown sensors",
			"refresh_time" : 30,
			"module_types" : [
				{
					"type" : "unknown",
					"attributes" : []
				},
				{
					"type" : "unknown",
					"attributes" : [
						{"attribute": "controllable"}
					]
				}
			]
		})

		self.ws.send(msg1)
		response = json.loads(self.ws.recv())

		self.assertEqual("generic_response", response["message_type"])
		self.assertEqual(id, response["id"])
		self.assertEqual(1, response["status"])
		assureNotClosed(self, self.ws)

		event = self.zmq.pop_data()
		self.assertEqual("on-new-device", event["event"])
		self.assertEqual("0xa123123412355431", event["device_id"])
		self.assertEqual("Device with unknown sensors", event["device_name"])
		self.assertEqual(2, len(event["modules"]))
		self.assertEqual("temperature", event["modules"][0]["type"])
		self.assertEqual(0, event["modules"][0]["controllable"])
		self.assertEqual("temperature", event["modules"][1]["type"])
		self.assertEqual(1, event["modules"][1]["controllable"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
