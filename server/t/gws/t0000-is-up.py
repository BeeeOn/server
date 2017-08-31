#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import websocket

class TestIsUp(unittest.TestCase):
	"""
	Connect to the gws server with TCP
	"""
	def test1_ws_port_is_open(self):
		s = socket.socket()
		s.connect((config.gws_host, config.gws_port))
		s.close()

	"""
	Connect to the gws server with WebSocket
	"""
	def test2_open_ws(self):
		ws = websocket.WebSocket()
		ws.connect(config.gws_ws_uri)
		ws.close()

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
