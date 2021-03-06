#! /usr/bin/env python3

import config
config.import_libs()

import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GatewayRegister, GatewayUnregister
from xmlui import LocationGetAll, LocationAdd, LocationDelete, LocationUpdate

class TestLocationGetUpdate(unittest.TestCase):
	"""
	Create a session, register a gateway and add some locations.
	"""
	def setUp(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_data())
		self.session = response.sessionid()

		response = c.request(GatewayRegister(
			config.tmp_gateway_id,
			self.session,
			name = "Gateway with locations"
		))
		self.assertTrue(response.is_ok())

		names = ["Livingroom", "Kitchen", "Bathroom"]
		self.locations = []

		for name in names:
			response = c.request(LocationAdd(
				config.tmp_gateway_id,
				self.session,
				name = name
			))
			self.assertTrue(response.is_data())
			self.assertEqual(1, len(response.root))
			self.locations.append(response.root[0].get("id"))

	"""
	Remove the locations, unregister the gateway and destroy the session.
	"""
	def tearDown(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		for location in self.locations:
			response = c.request(LocationDelete(
				config.tmp_gateway_id,
				location,
				self.session
			))
			self.assertTrue(response.is_ok())

		response = c.request(GatewayUnregister(
			config.tmp_gateway_id,
			self.session
		))
		self.assertTrue(response.is_ok())

		response = c.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	def test1_getall(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(LocationGetAll(
			config.tmp_gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(3, len(response.root))

		names = ["Livingroom", "Kitchen", "Bathroom"]
		for name in names:
			found = False
			for e in response.root:
				if e.get("name") == name:
					found = True
					break

			self.assertTrue(found, "missing: " + name)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
