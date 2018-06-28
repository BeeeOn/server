#! /usr/bin/env python3

import unittest

class TestSelf(unittest.TestCase):
	"""
	Test we are running and reporting proper output.
	"""
	def test1_self(self):
		return

	"""
	Check the prerequisites are available.
	"""
	def test2_prerequisites(self):
		import http.client
		import json
		import uuid
		import zmq

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
