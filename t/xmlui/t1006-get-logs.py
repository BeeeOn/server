#! /usr/bin/env python3

import config
config.import_libs()

from datetime import datetime
from datetime import timedelta
import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GatewayRegister, GatewayUnregister
from xmlui import DeviceGetLog

class TestDeviceGetLog(unittest.TestCase):
	"""
	Login and register gateway with sensors.
	"""
	def setUp(self):
		self.conn = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = self.conn.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_data())
		self.session = response.sessionid()

		response = self.conn.request(GatewayRegister(
			config.gateway_id,
			self.session,
			name = "Test Gateway"
		))

		self.assertTrue(response.is_ok())

	"""
	Unregister the gateway and logout.
	"""
	def tearDown(self):
		response = self.conn.request(GatewayUnregister(
			config.gateway_id,
			self.session
		))

		self.assertTrue(response.is_ok())

		response = self.conn.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	def test1_getlogs_intemp_1hour(self):
		response = self.conn.request(DeviceGetLog(
			config.gateway_id,
			"0xa335d00019f5234e",
			0,
			self.session,
			start = datetime.now() - timedelta(0, 60 * 60),
			end = datetime.now(),
			ftype = "avg"
		))

		self.assertTrue(response.is_data())
		self.assertEqual(14, len(response.root))

		for row in response.root:
			self.assertEqual("19.500000", row.text.split(" ")[1])

	def test2_getlogs_outtemp_2hours(self):
		response = self.conn.request(DeviceGetLog(
			config.gateway_id,
			"0xa335d00019f5234e",
			1,
			self.session,
			start = datetime.now() - timedelta(0, 24 * 60 * 60),
			end = datetime.now(),
			ftype = "avg"
		))

		self.assertTrue(response.is_data())
		self.assertEqual(65, len(response.root))

	def test2_getlogs_humidity_day(self):
		response = self.conn.request(DeviceGetLog(
			config.gateway_id,
			"0xa335d00019f5234e",
			2,
			self.session,
			start = datetime.now() - timedelta(0, 48 * 60 * 60),
			end = datetime.now(),
			ftype = "avg",
			interval = 300
		))

		self.assertTrue(response.is_data())
		self.assertEqual(240, len(response.root))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
