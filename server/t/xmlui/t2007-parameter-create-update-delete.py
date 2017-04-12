#! /usr/bin/env python3

import config
config.import_libs()

import unittest

from xmlui import Connector, Logout
from xmlui import GatewayRegister, GatewayUnregister
from xmlui import DeviceParameterGet
from xmlui import DeviceParameterCreate, DeviceParameterUpdate, DeviceParameterDelete

class TestCRUDeviceParameter(unittest.TestCase):
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

	"""
	Parameter 'invalid' (or any other garbage) cannot be created.
	"""
	def test1_create_invalid(self):
		response = self.c.request(DeviceParameterCreate(
			config.gateway_id,
			"0x4135d00019f5234e",
			"invalid",
			"value",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

		response = self.c.request(DeviceParameterCreate(
			config.gateway_id,
			"0x4135d00019f5234e",
			"garbage",
			"value",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

	"""
	Create IP address parameter for device 0x4135d00019f5234e.
	There is no such parameter defined thus it can be created.
	Finally, delete it to return to the previous state.
	"""
	def test2_create_delete_ip_address(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"ip-address",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("13", response.error_code())

		response = self.c.request(DeviceParameterCreate(
			config.gateway_id,
			"0x4135d00019f5234e",
			"ip-address",
			"10.0.0.6",
			self.session
		))

		self.assertTrue(response.is_ok())

		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"ip-address",
			self.session
		))

		self.assertTrue(response.is_data())
		self.assertEqual("10.0.0.6", response.root[0].get("parametervalue"))

		response = self.c.request(DeviceParameterDelete(
			config.gateway_id,
			"0x4135d00019f5234e",
			"ip-address",
			self.session
		))

		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"ip-address",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("13", response.error_code())

	"""
	Create password parameter for device 0x4135d00019f5234e.
	There is no such parameter defined thus it can be created.
	Finally, delete it to return to the previous state.
	"""
	def test3_create_password(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"password",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("13", response.error_code())

		response = self.c.request(DeviceParameterCreate(
			config.gateway_id,
			"0x4135d00019f5234e",
			"password",
			"top secret",
			self.session
		))

		self.assertTrue(response.is_ok())

		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"password",
			self.session
		))

		self.assertTrue(response.is_data())
		self.assertEqual("*****", response.root[0].get("parametervalue"))

		response = self.c.request(DeviceParameterDelete(
			config.gateway_id,
			"0x4135d00019f5234e",
			"password",
			self.session
		))

		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"password",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("13", response.error_code())

	"""
	Check value of IP address parameter of device 0x432d27aa5e94ecfd.
	Update it to a new value and check it was successful. Finally,
	revert the change back.
	"""
	def test4_update_ip_address(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"ip-address",
			self.session
		))
		
		self.assertTrue(response.is_data())
		self.assertEqual("10.0.0.1", response.root[0].get("parametervalue"))

		response = self.c.request(DeviceParameterUpdate(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"ip-address",
			"192.168.1.2",
			self.session
		))

		self.assertTrue(response.is_ok())

		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"ip-address",
			self.session
		))
		
		self.assertTrue(response.is_data())
		self.assertEqual("192.168.1.2", response.root[0].get("parametervalue"))

		response = self.c.request(DeviceParameterUpdate(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"ip-address",
			"10.0.0.1",
			self.session
		))

		self.assertTrue(response.is_ok())

	"""
	Check value of IP address parameter of device 0x432d27aa5e94ecfd.
	Delete it and then re-created it again.
	"""
	def test5_delete_ip_address(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"ip-address",
			self.session
		))
		
		self.assertTrue(response.is_data())
		self.assertEqual("10.0.0.1", response.root[0].get("parametervalue"))

		response = self.c.request(DeviceParameterDelete(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"ip-address",
			self.session
		))

		self.assertTrue(response.is_ok())

		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"ip-address",
			self.session
		))
		
		self.assertTrue(response.is_error())
		self.assertEqual("13", response.error_code())

		response = self.c.request(DeviceParameterCreate(
			config.gateway_id,
			"0x432d27aa5e94ecfd",
			"ip-address",
			"10.0.0.1",
			self.session
		))

		self.assertTrue(response.is_ok())

	"""
	Device 0x4471959aad24618e does not contain firmware but we cannot
	create it via this interface anyway.
	"""
	def test6_create_undefined_firmware_fails(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4471959aad24618e",
			"firmware",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("13", response.error_code())

		response = self.c.request(DeviceParameterCreate(
			config.gateway_id,
			"0x4471959aad24618e",
			"firmware",
			"v2000",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

	"""
	Device 0x4135d00019f5234e contains firmware but we cannot
	update it via this interface anyway.
	"""
	def test7_update_firmware_fails(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"firmware",
			self.session
		))

		self.assertTrue(response.is_data())

		response = self.c.request(DeviceParameterUpdate(
			config.gateway_id,
			"0x4135d00019f5234e",
			"firmware",
			"v2001",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

	"""
	Device 0x4135d00019f5234e contains firmware but we cannot
	delete it via this interface anyway.
	"""
	def test8_update_firmware_fails(self):
		response = self.c.request(DeviceParameterGet(
			config.gateway_id,
			"0x4135d00019f5234e",
			"firmware",
			self.session
		))

		self.assertTrue(response.is_data())

		response = self.c.request(DeviceParameterDelete(
			config.gateway_id,
			"0x4135d00019f5234e",
			"firmware",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
