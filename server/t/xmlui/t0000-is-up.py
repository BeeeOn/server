#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
from xmlui import Connector, Request, Response, Login

class TestIsUp(unittest.TestCase):
	def setUp(self):
		self.c = Connector(config.xmlui_host, config.xmlui_port)

	"""
	Ping the xmlui server port.
	"""
	def test1_port_is_open(self):
		s = socket.socket()
		s.connect((config.xmlui_host, config.xmlui_port))
		s.close()

	"""
	Send incomplete request.
	"""
	def test2_incomplete_request(self):
		response = self.c.request(Request())
		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

	"""
	Send failing login request.
	"""
	def test3_login_with_invalid_provider(self):
		response = self.c.request(Login("invalid", "nouser"))
		self.assertTrue(response.is_error())
		self.assertEqual("20", response.error_code())

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
