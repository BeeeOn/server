#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
from rest import GET, POST, PUT

class TestIsUp(unittest.TestCase):
	"""
	Ping the ui server port.
	"""
	def test1_port_is_open(self):
		s = socket.socket()
		s.connect((config.ui_host, config.ui_port))
		s.close()

	"""
	Send GET to /nothing. Such method and path is not supported.
	"""
	def test2_http_auth_GET_404(self):
		req = GET(config.ui_host, config.ui_port, "/nothing")
		response, _ = req()
		self.assertEqual(404, response.status)

	"""
	Send POST to /auth. No authorization info is attached.
	"""
	def test3_http_auth_POST_401(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		req.body("")
		response, _ = req()
		self.assertEqual(401, response.status)

	"""
	Send PUT to /auth. Such method is not supported on this path.
	"""
	def test4_http_auth_PUT_404(self):
		req = PUT(config.ui_host, config.ui_port, "/auth")
		response, _ = req()
		self.assertEqual(404, response.status)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
