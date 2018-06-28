#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json
import uuid

from gws import assureIsClosed, assureNotClosed, registerGateway, ZMQConnection

class TestNewDeviceGroup(unittest.TestCase):
	def setUp(self):
		self.zmq = ZMQConnection(config.gws_zmq_endpoint)
		self.zmq.open()

		self.ws = websocket.WebSocket()
		self.ws.connect(config.gws_ws_uri)

		registerGateway(self, self.ws, config.gateway_id)

		event = self.zmq.pop_data(timeout = 5)
		self.assertEqual("on-connected", event["event"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

	def tearDown(self):
		self.ws.close()

		try:
			event = self.zmq.pop_data(timeout = 5)
			self.assertEqual("on-disconnected", event["event"])
			self.assertEqual(config.gateway_id, event["gateway_id"])
		finally:
			self.zmq.close()

	"""
	Register device group with valid types. It should be accepted without any issue.
	"""
	def test1_new_device_group_success(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_group_request",
			"id" : id,
			"group_name" : "BeeeOn Group",
			"vendor" : "BeeeOn",
			"devices" : [
				{
					"device_id" : "0xa100998877665511",
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
				},
				{
					"device_id" : "0xa100998877665522",
					"product_name" : "Temperature Humidity v1.0",
					"refresh_time" : 30,
					"module_types" : [
						{
							"type" : "temperature",
							"attributes" : [
								{"attribute" : "inner"}
							]
						},
						{
							"type" : "temperature",
							"attributes" : [
								{"attribute" : "outer"}
							]
						},
						{
							"type" : "humidity",
							"attributes" : [
								{"attribute" : "inner"}
							]
						},
						{
							"type" : "battery",
							"attributes" : []
						},
						{
							"type" : "rssi",
							"attributes" : []
						}
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

		event = self.zmq.pop_data(timeout = 5)
		self.assertEqual("on-new-device", event["event"])
		self.assertEqual("0xa100998877665511", event["device_id"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

		event = self.zmq.pop_data(timeout = 5)
		self.assertEqual("on-new-device", event["event"])
		self.assertEqual("0xa100998877665522", event["device_id"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

	"""
	Register device that does not match to any types known on the
	server. The server must fail to register this devices and provide
	an appropriate response. The response has to have same id as the
	request and the status of response has to be 2 (failure).
	"""
	def test2_new_device_group_fail_invalid_product(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_group_request",
			"id" : id,
			"group_name" : "Awesome Group",
			"vendor" : "Good Company",
			"devices" : [
				{
					"device_id" : "0xa100998877665533",
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
				}
			]
		})

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("generic_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(2, msg["status"])
		assureNotClosed(self, self.ws)

		event = self.zmq.pop_data(timeout = 5)
		self.assertEqual("on-refused-new-device", event["event"])
		self.assertEqual("0xa100998877665533", event["device_id"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

	"""
	Server must deny accepting of device with an invalid device ID.
	The type of the device would be valid and acceptable otherwise.
	"""
	def test3_new_device_group_disconnect_invalid_id(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_group_request",
			"id" : id,
			"group_name" : "Group",
			"vendor" : "BeeeOn",
			"devices" : [
				{
					"device_id" : "123",
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
				}
			]
		})

		self.ws.send(msg)
		assureIsClosed(self, self.ws)

	"""
	Register device whose module type 'humidity' does not match with the
	known module 'pressure' for that device type on the server. The server
	must fail to register this devices and provide an appropriate response.
	The response has to have same id as the request and the status of response
	has to be 2 (failure).
	"""
	def test4_new_device_group_fail_invalid_module(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_group_request",
			"id" : id,
			"group_name" : "Group",
			"vendor" : "BeeeOn",
			"devices" : [
				{
					"device_id" : "0xa100998877665544",
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
				}
			]
		})

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("generic_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(2, msg["status"])
		assureNotClosed(self, self.ws)

		event = self.zmq.pop_data(timeout = 5)
		self.assertEqual("on-refused-new-device", event["event"])
		self.assertEqual("0xa100998877665544", event["device_id"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

	"""
	Register group of 2 devices. The first device however contains a module
	that is not known to the server. The server must fail to register these devices
	and provide an appropriate response. The response has to have same id as the
	request and the status of response has to be 2 (failure).
	"""
	def test5_new_device_group_fail_too_many_modules(self):
		id = str(uuid.uuid4())

		msg = json.dumps({
			"message_type" : "new_device_group_request",
			"id" : id,
			"group_name" : "BeeeOn Group",
			"vendor" : "BeeeOn",
			"devices" : [
				{
					"device_id" : "0xa100998877665555",
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
				},
				{
					"device_id" : "0xa100998877665566",
					"product_name" : "Temperature Humidity v1.0",
					"refresh_time" : 30,
					"module_types" : [
						{
							"type" : "temperature",
							"attributes" : [
								{"attribute" : "inner"}
							]
						},
						{
							"type" : "temperature",
							"attributes" : [
								{"attribute" : "outer"}
							]
						},
						{
							"type" : "humidity",
							"attributes" : [
								{"attribute" : "inner"}
							]
						},
						{
							"type" : "battery",
							"attributes" : []
						},
						{
							"type" : "rssi",
							"attributes" : []
						}
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

		event = self.zmq.pop_data(timeout = 5)
		self.assertEqual("on-refused-new-device", event["event"])
		self.assertEqual("0xa100998877665555", event["device_id"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

		event = self.zmq.pop_data(timeout = 5)
		self.assertEqual("on-refused-new-device", event["event"])
		self.assertEqual("0xa100998877665566", event["device_id"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
