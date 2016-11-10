#! /usr/bin/env python3

import config
config.import_libs()

import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GatewayRegister, GatewayUnregister
from xmlui import DeviceGetAll, DeviceGetNew, DeviceGet
from xmlui import DeviceUpdate, DeviceUnregister
from xmlui import LocationAdd, LocationDelete

class TestDeviceUpdateActivate(unittest.TestCase):
	"""
	Create a session, register a well-known gateway with
	assigned devices. Check we have 3 devices available for
	testing and store their EUIDs, and 1 inactive device.
	"""
	def setUp(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_data())
		self.session = response.sessionid()

		response = c.request(GatewayRegister(
			config.gateway_id,
			self.session,
			name = "Gateway with devices"
		))
		self.assertTrue(response.is_ok())

		response = c.request(DeviceGetAll(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(3, len(response.root))

		self.devices = []
		for e in response.root:
			self.devices.append(e.get("euid"))

		response = c.request(DeviceGetNew(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		self.inactive = response.root[0].get("euid")

	"""
	Assure, we have 3 devices active and 1 inactive as it was
	assumed on setUp. Unregister the gateway and destroy the session.
	"""
	def tearDown(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(DeviceGetAll(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(3, len(response.root))

		response = c.request(DeviceGetNew(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		response = c.request(GatewayUnregister(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_ok())

		response = c.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	"""
	Update a non-existing device should fail.
	"""
	def test1_update_non_existing(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		# assure such device does not exist
		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			"1030123102"
		))
		self.assertTrue(response.is_data())
		self.assertEqual(0, len(response.root))

		# update non-existing device
		response = c.request(DeviceUpdate(
			config.gateway_id,
			"1030123102",
			self.session,
			name = "NO SUCH DEVICE"
		))
		self.assertTrue(response.is_error())
		self.assertEqual("1001", response.error_code())

		# assure such device was not accidently created
		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			"1030123102"
		))
		self.assertTrue(response.is_data())
		self.assertEqual(0, len(response.root))

	"""
	Change name of an existing device and revert it back.
	"""
	def test2_update_existing(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		# assure such device exists
		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			self.devices[0]
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		data = response.root[0]
		self.assertNotEqual("UPDATE", data.get("name"))

		response = c.request(DeviceUpdate(
			config.gateway_id,
			self.devices[0],
			self.session,
			name = "UPDATE"
		))
		self.assertTrue(response.is_ok())

		# test update was successful
		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			self.devices[0]
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		self.assertEqual("UPDATE", response.root[0].get("name"))

		# revert name
		response = c.request(DeviceUpdate(
			config.gateway_id,
			self.devices[0],
			self.session,
			name = data.get("name")
		))
		self.assertTrue(response.is_ok())

	"""
	Send update of a device to set a location.
	"""
	def test3_update_location(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(LocationAdd(
			config.gateway_id,
			self.session,
			name = "Livingroom"
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		location = response.root[0]

		response = c.request(DeviceUpdate(
			config.gateway_id,
			self.devices[1],
			self.session,
			locationid = location.get("id")
		))
		self.assertTrue(response.is_ok())

		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			self.devices[1]
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		self.assertEqual(location.get("id"),
				response.root[0].get("locationid"))

		response = c.request(DeviceUpdate(
			config.gateway_id,
			self.devices[1],
			self.session,
			locationid = "00000000-0000-0000-0000-000000000000"
		))
		self.assertTrue(response.is_ok())

		response = c.request(LocationDelete(
			config.gateway_id,
			location.get("id"),
			self.session
		))
		self.assertTrue(response.is_ok())

	"""
	Test activation of the inactive device. It is temporarily
	activated and renamed during this test.
	"""
	def test4_activate(self):
		c = Connector(config.xmlui_host, config.xmlui_port)

		response = c.request(DeviceGetNew(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		e = response.root[0]
		self.assertEqual("0", e.get("init"))
		self.assertEqual("Unknown", e.get("name"))
		self.assertIsNotNone(e.get("euid", None))

		response = c.request(DeviceUpdate(
			config.gateway_id,
			e.get("euid"),
			self.session,
			name = "My favourite device"
		))
		self.assertTrue(response.is_ok())

		response = c.request(DeviceGetNew(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(0, len(response.root))

		response = c.request(DeviceGetAll(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(4, len(response.root))

		names = ["Temperature", "Humidity", "Multi-sensor",
				"My favourite device"]
		for name in names:
			found = False
			for e in response.root:
				if e.get("name") == name:
					found = True
					break

			self.assertTrue(found, "missing: " + name)

		# revert the update of name
		response = c.request(DeviceUpdate(
			config.gateway_id,
			e.get("euid"),
			self.session,
			name = "Unknown"
		))
		self.assertTrue(response.is_ok())

		response = c.request(DeviceUnregister(
			config.gateway_id,
			e.get("euid"),
			self.session
		))
		self.assertTrue(response.is_ok())

		response = c.request(DeviceGetNew(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		response = c.request(DeviceGetAll(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(3, len(response.root))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
