#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json

from gws import assureIsClosed, assureNotClosed, registerGateway, ZMQConnection

class TestControlFrames(unittest.TestCase):
	def setUp(self):
		self.zmq = ZMQConnection(config.gws_zmq_endpoint)
		self.zmq.accept(lambda m: "gateway_id" in m)
		self.zmq.open()

		self.ws = websocket.WebSocket()
		self.ws.connect(config.gws_ws_uri)

		registerGateway(self, self.ws, config.gateway_id)

		event = self.zmq.pop_data(timeout = 10)
		self.assertEqual("on-connected", event["event"])
		self.assertEqual(config.gateway_id, event["gateway_id"])

	def tearDown(self):
		self.ws.close()

		try:
			event = self.zmq.pop_data(timeout = 10)
			self.assertEqual("on-disconnected", event["event"])
			self.assertEqual(config.gateway_id, event["gateway_id"])
		finally:
			self.zmq.close()

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

	"""
	Valid PING frame should lead to an appropriate PONG response.
	"""
	def test4_ping_pong(self):
		self.ws.ping("testing connection")
		frame = self.ws.recv_frame()

		self.assertEqual(websocket.ABNF.OPCODE_PONG, frame.opcode)
		self.assertEqual("testing connection", str(frame.data, "utf-8"))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
