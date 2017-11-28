#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json

from gws import assureIsClosed, assureNotClosed

class TestControlFrames(unittest.TestCase):
	def setUp(self):
		self.ws = websocket.WebSocket()
		self.ws.connect(config.gws_ws_uri)

		msg = json.dumps(
			{
				"gateway_id": config.gateway_id,
				"ip_address": "192.168.1.1",
				"message_type": "gateway_register",
				"version": "v1.0"
			}
		)

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("gateway_accepted", msg["message_type"])

	def tearDown(self):
		self.ws.close()

	"""
	Opcode PONG is not processed on server and any such frame should lead
	to connection close. This should happen for any other unhandled control
	frame.
	"""
	def test1_unhandled_control(self):
		self.ws.pong("short but should close connection")
		assureIsClosed(self, self.ws)

	"""
	Opcode CONT is not processed by server. It just leads to closing the
	connection. There is no way how to determine the reason from the server.
	"""
	def test2_unhandled_data(self):
		self.ws.send("should close connection due to opcode",
				opcode = websocket.ABNF.OPCODE_CONT)
		assureIsClosed(self, self.ws)

	"""
	Control opcodes must be max 125 B long. All such frames must be ignored
	on server and such connection would be closed.
	"""
	def test3_too_long_frame(self):
		self.ws.ping("X" * 1024)
		assureIsClosed(self, self.ws)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
