#! /usr/bin/env python3

import config
config.import_libs()

import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GatewayRegister, GatewayUnregister
from xmlui import GatewayGet, GatewayUpdate, GatewayGetAll

class TestGatewayGetUpdate(unittest.TestCase):
	"""
	Create a session and register a gateway.
	"""
	def setUp(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_ok())
		self.session = response.sessionid()

		# assure we have no gateway associated
		response = c.request(GatewayGetAll(self.session))
		self.assertTrue(response.is_data())
		self.assertEqual(0, len(response.root))

		response = c.request(GatewayRegister(
			config.gateway_id,
			self.session,
			name = "Test Gateway First Registered"
		))

		self.assertTrue(response.is_ok())

	"""
	Unregister the gateway and destroy the session.
	"""
	def tearDown(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(GatewayUnregister(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_ok())

		response = c.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	"""
	Try to fetch a non-existing gateway.
	"""
	def test0_get_non_existing(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(GatewayGet(
			"1464123670904752",
			self.session
		))
		self.assertTrue(response.is_error())
		self.assertEqual("1001", response.error_code())

	"""
	Request an existing gateway.
	"""
	def test1_get_existing(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(GatewayGet(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())

		self.assertEqual("Test Gateway First Registered",
				response.root[0].get("name"))
		self.assertEqual(config.gateway_id,
				response.root[0].get("id"))
		self.assertEqual("nan", response.root[0].get("latitude"))
		self.assertEqual("nan", response.root[0].get("longitude"))
		self.assertEqual("nan", response.root[0].get("altitude"))

	"""
	Get the single implicit gateway by getall.
	"""
	def test2_get_all_single(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(GatewayGetAll(self.session))
		self.assertTrue(response.is_data())

		self.assertEqual(1, len(response.root))
		self.assertEqual("Test Gateway First Registered",
				response.root[0].get("name"))

	"""
	Register another gateway and get both by getall.
	"""
	def test3_get_all(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(GatewayRegister(
			config.tmp_gateway_id,
			self.session,
			name = "Another Gateway"
		))

		self.assertTrue(response.is_ok())

		response = c.request(GatewayGetAll(self.session))
		self.assertTrue(response.is_data())

		self.assertEqual(2, len(response.root))

		if response.root[0].get("id") == config.tmp_gateway_id:
			self.assertEqual("Another Gateway",
				response.root[0].get("name"))
			self.assertEqual("Test Gateway First Registered",
				response.root[1].get("name"))
		else:
			self.assertEqual("Another Gateway",
				response.root[1].get("name"))
			self.assertEqual("Test Gateway First Registered",
				response.root[0].get("name"))

		response = c.request(GatewayUnregister(
			config.tmp_gateway_id,
			self.session
		))
		self.assertTrue(response.is_ok())

	def test4_test_update(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(GatewayUpdate(
			config.gateway_id,
			self.session,
			name = "Updated Gateway",
			altitude = 10,
			latitude = 15,
			longitude = 20
		))

		self.assertTrue(response.is_ok())

		response = c.request(GatewayGet(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())

		self.assertEqual("Updated Gateway",
				response.root[0].get("name"))
		self.assertEqual(10, float(response.root[0].get("altitude")))
		self.assertEqual(15, float(response.root[0].get("latitude")))
		self.assertEqual(20, float(response.root[0].get("longitude")))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))

