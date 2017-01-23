#! /usr/bin/env python3

import config
config.import_libs()

import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GatewayRegister, GatewayUnregister, GatewayGet

class TestGatewayRegisterUnregister(unittest.TestCase):
	"""
	Create 2 sessions for 2 different users.
	"""
	def setUp(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_data())
		self.session = response.sessionid()

		response = c.request(config.PERMIT_LOGIN2)
		self.assertTrue(response.is_data())
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

	"""
	Test register and unregister the well-known virtual testing
	gateway	1780053541714013. Assume there is an implicit place
	created for the user.
	"""
	def test1_register_unregister(self):
		c = Connector(config.xmlui_host, config.xmlui_port)
		response = c.request(GatewayRegister(
			config.tmp_gateway_id,
			self.session,
			name = "Test Gateway Registered"
		))

		self.assertTrue(response.is_ok())

		response = c.request(GatewayGet(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_data())
		self.assertEqual("Test Gateway Registered",
				response.root[0].get("name"))
		self.assertEqual("1780053541714013",
				response.root[0].get("id"))
		self.assertEqual("nan", response.root[0].get("latitude"))
		self.assertEqual("nan", response.root[0].get("longitude"))
		self.assertEqual("nan", response.root[0].get("altitude"))
		self.assertEqual("0", response.root[0].get("devices"))
		self.assertEqual("1", response.root[0].get("users"))
		self.assertEqual("", response.root[0].get("version"))
		self.assertEqual("0.0.0.0", response.root[0].get("ip"))
		self.assertEqual("admin", response.root[0].get("permission"))
		self.assertEqual("Joe Doe", response.root[0].get("owner"))
		self.assertEqual("0", response.root[0].get("timezone"))
		self.assertEqual("available", response.root[0].get("status"))

		response = c.request(GatewayUnregister(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_ok())

		response = c.request(GatewayGet(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("1001", response.error_code())

	"""
	Test cannot register an already registered gateway. To register
	a gateway by a different user, it is necessary to unregister it
	first.
	"""
	def test2_cannot_steal_gateway(self):
		c = Connector(config.xmlui_host, config.xmlui_port)
		response = c.request(GatewayRegister(
			config.tmp_gateway_id,
			self.session,
			name = "Test Gateway First Registered"
		))

		self.assertTrue(response.is_ok())

		response = c.request(GatewayGet(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_data())
		self.assertEqual("Test Gateway First Registered",
				response.root[0].get("name"))

		# try to steal by the second user
		response = c.request(GatewayRegister(
			config.tmp_gateway_id,
			self.session2,
			name = "Test Gateway Second Registered"
		))

		self.assertTrue(response.is_error())
		self.assertEqual("13", response.error_code())

		# check the gateway was not changed
		response = c.request(GatewayGet(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_data())
		self.assertEqual("Test Gateway First Registered",
				response.root[0].get("name"))
		self.assertEqual("1780053541714013",
				response.root[0].get("id"))
		self.assertEqual("nan", response.root[0].get("latitude"))
		self.assertEqual("nan", response.root[0].get("longitude"))
		self.assertEqual("nan", response.root[0].get("altitude"))
		self.assertEqual("0", response.root[0].get("devices"))
		self.assertEqual("1", response.root[0].get("users"))
		self.assertEqual("", response.root[0].get("version"))
		self.assertEqual("0.0.0.0", response.root[0].get("ip"))
		self.assertEqual("admin", response.root[0].get("permission"))
		self.assertEqual("Joe Doe", response.root[0].get("owner"))
		self.assertEqual("0", response.root[0].get("timezone"))
		self.assertEqual("available", response.root[0].get("status"))

		response = c.request(GatewayUnregister(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_ok())

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
