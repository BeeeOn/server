#! /usr/bin/env python3

import config
config.import_libs()

import unittest

from xmlui import Connector, Logout
from xmlui import GatewayRegister, GatewayUnregister
from xmlui import DeviceParameterGet

class TestGetDeviceParameter(unittest.TestCase):
	"""
	Create a session, register a well-known gateway with
	assigned devices.
	"""
	def setUp(self):
		self.c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = self.c.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_data())
		self.session = response.sessionid()

		response = self.c.request(GatewayRegister(
			config.gateway_id,
			self.session,
			name = "Gateway with devices"
		))
		self.assertTrue(response.is_ok())

	"""
	Unregister the gateway and destroy the session.
	"""
	def tearDown(self):
		response = self.c.request(GatewayUnregister(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_ok())

		response = self.c.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	def test1_get_invalid_parameter(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"testing",
			self.session
		))
		
		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

	"""
	Testing devices 0x4135d00019f5234e and 0x427e0f7f0302324d provide the
	parameter 'firmware'. Check we can read it.
	"""
	def test2_get_firmware(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"firmware",
			self.session
		))
		
		self.assertTrue(response.is_data())
		self.assertEqual("v1.0-6453", response.root[0].get("parametervalue"))

		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x427e0f7f0302324d",
			"firmware",
			self.session
		))
		
		self.assertTrue(response.is_data())
		self.assertEqual("000-111", response.root[0].get("parametervalue"))

	"""
	Testing device 0x432d27aa5e94ecfd provides parameter 'ip-address'.
	Check that the IP address can be read.
	"""
	def test3_get_ip_address(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"ip-address",
			self.session
		))
		
		self.assertTrue(response.is_data())
		self.assertEqual("10.0.0.1", response.root[0].get("parametervalue"))

	"""
	Testing device 0x432d27aa5e94ecfd provides parameter 'password'.
	Check that the password cannot be read this way, only placeholders
	('*****') are returned to denote the password exists.
	"""
	def test4_get_password(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"password",
			self.session
		))
		
		self.assertTrue(response.is_data())
		self.assertEqual("*****", response.root[0].get("parametervalue"))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
