#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json
import uuid

from gws import assureIsClosed, assureNotClosed, registerGateway, ZMQConnection

class TestDeviceList(unittest.TestCase):
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
	Gateway 1284174504043136 has already associated 4 devices via the testing-data.sql
	init script. 3 of them are active (paired). Ask server for the paired virtual devices
	and test it give us them all.
	"""
	def test1_device_list_success_3_virtual(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"message_type" : "device_list_request",
				"id" : id,
				"device_prefix" : "VirtualDevice"
			}
		)

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("device_list_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(1, msg["status"])
		self.assertEqual(3, len(msg["devices"]))
		self.assertEqual("0xa32d27aa5e94ecfd", msg["devices"][0]["device_id"])
		self.assertEqual("0xa335d00019f5234e", msg["devices"][1]["device_id"])
		self.assertEqual("0xa37e0f7f0302324d", msg["devices"][2]["device_id"])
		assureNotClosed(self, self.ws)

	"""
	The gateway_id 1284174504043136 has no Jablotron device registered. Test we
	are given no such device by the server.
	"""
	def test2_device_list_success_0_jablotron(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"message_type" : "device_list_request",
				"id" : id,
				"device_prefix" : "Jablotron"
			}
		)

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("device_list_response", msg["message_type"])
		self.assertEqual(id, msg["id"])
		self.assertEqual(1, msg["status"])
		self.assertEqual(0, len(msg["devices"]))
		assureNotClosed(self, self.ws)

	"""
	When asking for devices of an unknown device prefix, the server encounters
	an exception which leads to closing the connection. This behaviour would be
	changed in the future. Now, make sure that an unknown prefix breaks the
	connection of the gateway.
	"""
	def test3_device_list_disconnect_bad_prefix(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"message_type" : "device_list_request",
				"id" : id,
				"device_prefix" : "Cool"
			}
		)

		self.ws.send(msg)
		assureIsClosed(self, self.ws)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
