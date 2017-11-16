#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import POST

class TestLocalization(unittest.TestCase):
	def test1_error_cs(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		req["Accept-Language"] = "cs-CZ"
		response, content = req()

		self.assertEqual(401, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])
		self.assertEqual(401, result["code"])
		self.assertEqual("uživatel nepřihlášen", result["message"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
