#! /usr/bin/env python3

import config
config.import_libs()

import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GetMyProfile

class TestGetMyProfile(unittest.TestCase):
	"""
	Create 2 sessions for 2 different users.
	"""
	def setUp(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_ok())
		self.session = response.sessionid()

		response = c.request(config.PERMIT_LOGIN2)
		self.assertTrue(response.is_ok())
		self.session2 = response.sessionid()

	"""
	Destroy the sessions.
	"""
	def tearDown(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(Logout(self.session2))
		self.assertTrue(response.is_ok())

		response = c.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	def test0_getmyprofile_first(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(GetMyProfile(self.session))
		self.assertTrue(response.is_data())

		self.assertEqual("joe.doe@example.org",
				response.root[0].get("email"))
		self.assertEqual("Joe", response.root[0].get("name"))
		self.assertEqual("Joe", response.root[0].get("first_name"))
		self.assertEqual("Doe", response.root[0].get("surname"))
		self.assertEqual("Doe", response.root[0].get("last_name"))
		self.assertEqual("unknown", response.root[0].get("gender"))

	def test1_getmyprofile_second(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(GetMyProfile(self.session2))
		self.assertTrue(response.is_data())

		self.assertEqual("john.smith@example.org",
				response.root[0].get("email"))
		self.assertEqual("John", response.root[0].get("name"))
		self.assertEqual("John", response.root[0].get("first_name"))
		self.assertEqual("Smith", response.root[0].get("surname"))
		self.assertEqual("Smith", response.root[0].get("last_name"))
		self.assertEqual("unknown", response.root[0].get("gender"))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
