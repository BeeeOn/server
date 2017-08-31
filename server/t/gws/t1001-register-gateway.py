#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json

from gws import assureIsClosed, assureNotClosed

class TestRegister(unittest.TestCase):
	"""
	Register the gateway with a valid registration message. Expect successful
	registration and open connection.
	"""
	def test1_register_success(self):
		ws = websocket.WebSocket()
		ws.connect(config.gws_ws_uri)

		msg = json.dumps(
			{
				"gateway_id" : config.gateway_id,
				"ip_address" : "192.168.1.1",
				"message_type" : "gateway_register",
				"version" : "v1.0"
			}
		)

		ws.send(msg)
		msg = json.loads(ws.recv())

		self.assertEqual("gateway_accepted", msg["message_type"])
		assureNotClosed(self, ws)
		ws.close()

	"""
	Register the gateway with invalid id and expect closed connection.
	"""
	def test2_register_fail_bad_id(self):
		ws = websocket.WebSocket()
		ws.connect(config.gws_ws_uri)

		msg = json.dumps(
			{
				"gateway_id" : "123",
				"ip_address" : "192.168.1.1",
				"message_type" : "gateway_register",
				"version" : "v1.0"
			}
		)

		ws.send(msg)
		assureIsClosed(self, ws)

	"""
	Send invalid message before registration and expect closed connection.
	"""
	def test3_send_random_data_without_registration(self):
		ws = websocket.WebSocket()
		ws.connect(config.gws_ws_uri)

		ws.send("hello")

		assureIsClosed(self, ws)
		ws.close()

	"""
	Register the same gateway twice without closing first connection. Expect,
	that the second registration closes the first connection.
	"""
	def test4_register_success_repeatedly(self):
		ws0 = websocket.WebSocket()
		ws0.connect(config.gws_ws_uri)

		msg = json.dumps(
			{
				"gateway_id" : config.gateway_id,
				"ip_address" : "192.168.1.1",
				"message_type" : "gateway_register",
				"version" : "v1.0"
			}
		)

		ws0.send(msg)
		msg = json.loads(ws0.recv())

		self.assertEqual("gateway_accepted", msg["message_type"])
		assureNotClosed(self, ws0)

		ws1 = websocket.WebSocket()
		ws1.connect(config.gws_ws_uri)

		msg = json.dumps(
			{
				"gateway_id" : config.gateway_id,
				"ip_address" : "192.168.1.1",
				"message_type" : "gateway_register",
				"version" : "v1.0"
			}
		)

		ws1.send(msg)
		msg = json.loads(ws1.recv())

		self.assertEqual("gateway_accepted", msg["message_type"])
		assureIsClosed(self, ws0)
		assureNotClosed(self, ws1)

		ws1.close()

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
