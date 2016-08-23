#! /usr/bin/env python3

import unittest

class TestSelf(unittest.TestCase):
	"""
	Test we are running and reporting proper output.
	"""
	def test1_self(self):
		return

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
