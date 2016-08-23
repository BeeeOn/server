#! /usr/bin/env python3

import unittest
import socket
import ui.config

POST_WITHOUT_CONTENT_LENGTH = "\r\n".join([
	"POST /auth HTTP/1.1",
	"Host: " + ui.config.ui_host + ":" + str(ui.config.ui_port),
	"\r\n"
])

class TestNoContentLength(unittest.TestCase):
	"""
	Test behaviour of the server when the Content-Length header
	is not send with POST. The server should not stall on that
	request and return immediatelly 400.
	"""
	def test1_ui_no_content_length(self):
		s = socket.socket()
		s.connect((ui.config.ui_host, ui.config.ui_port))
		s.settimeout(1)
		s.sendall(bytes(POST_WITHOUT_CONTENT_LENGTH, "utf-8"))
		data = str(s.recv(4096), "utf-8").split("\r\n")
		s.close()
		self.assertEquals("HTTP/1.1 400 Bad Request", data[0])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
