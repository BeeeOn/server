#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import POST

class TestTooLongInput(unittest.TestCase):
	"""
	Test behaviour of the server when the Content-Length header
	contains a very big value on POST. The assumed limit of the
	server is 10 kB.
	"""
	def test1_too_big_content_length(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		req.body("x" * (1024 * 1024))
		response, content = req()

		self.assertEqual(400, response.status)
		self.assertEqual(0, len(content))

	"""
	Send too long input with a fake Content-Length header.
	"""
	def test2_too_long_input(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		login = json.loads(config.PERMIT_LOGIN)
		login["bloat"] = ("x" * (1024 * 1024))
		req.body(json.dumps(login))
		req["Content-Length"] = 1024
		response, content = req()

		self.assertEqual(401, response.status)

		result = json.loads(content)
		self.assertEqual("error", result["status"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
