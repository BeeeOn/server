#! /usr/bin/env python3

import config
config.import_libs()

import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GatewayRegister, GatewayUnregister
from xmlui import LocationAdd, LocationDelete, LocationGetAll

class TestLocationAddDelete(unittest.TestCase):
	"""
	Create a session and a gateway.
	"""
	def setUp(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_data())
		self.session = response.sessionid()

		response = c.request(GatewayRegister(
			config.gateway_id,
			self.session,
			name = "Gateway For Locations"
		))

		self.assertTrue(response.is_ok())

	"""
	Destroy the session.
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
	Add location to the prepared gateway and then delete it.
	"""
	def test1_add_delete(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(LocationAdd(
			config.gateway_id,
			self.session,
			name = "Livingroom"
		))
		self.assertTrue(response.is_ok())

		response = c.request(LocationGetAll(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))
		self.assertEqual("Livingroom", response.root[0].get("name"))

		response = c.request(LocationDelete(
			config.gateway_id,
			response.root[0].get("locationid"),
			self.session
		))

		response = c.request(LocationGetAll(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(0, len(response.root))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
