#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json
import uuid

from gws import assureIsClosed, assureNotClosed, registerGateway, ZMQConnection

class TestLastValue(unittest.TestCase):
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
	Obtain last value of the device's 0xa32d27aa5e94ecfd (11758097814818974973)
	module 0. The last value of such sensor is 20.0 (see testing-data.sql).
	"""
	def test1_last_value_success(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"message_type" : "last_value_request",
				"id" : id,
				"device_id" : "0xa32d27aa5e94ecfd",
				"module_id" : 0
			}
		)

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("last_value_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(1, msg["status"])
		self.assertEqual(True, msg["valid"])
		self.assertEqual(20.0, msg["value"])
		assureNotClosed(self, self.ws)

	"""
	Obtain last value of the device's 0xa32d27aa5e94ecfd (11758097814818974973)
	module 1. There is no last value of that module, expect an error response.
	"""
	def test2_last_value_fail(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"message_type" : "last_value_request",
				"id" : id,
				"device_id" : "0xa32d27aa5e94ecfd",
				"module_id" : 1
			}
		)

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("last_value_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(2, msg["status"])
		assureNotClosed(self, self.ws)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
