#! /usr/bin/env python3

import config
config.import_libs()

import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GatewayRegister, GatewayUnregister
from xmlui import DeviceGetAll, DeviceGetNew, DeviceGet

class TestDeviceGet(unittest.TestCase):
	"""
	Create a session, register a well-known gateway with
	assigned devices.
	"""
	def setUp(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_data())
		self.session = response.sessionid()

		response = c.request(GatewayRegister(
			config.gateway_id,
			self.session,
			name = "Gateway with devices"
		))
		self.assertTrue(response.is_ok())


	"""
	Unregister the gateway and destroy the session.
	"""
	def tearDown(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(GatewayUnregister(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_ok())

		response = c.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	def assertDeviceIsComplete(self, e):
		self.assertIsNotNone(e.get("init", None))
		self.assertIsNotNone(e.get("gateid", None))
		self.assertIsNotNone(e.get("id", None))
		self.assertIsNotNone(e.get("euid", None))
		self.assertIsNotNone(e.get("time", None))
		self.assertIsNotNone(e.get("type", None))
		self.assertIsNotNone(e.get("locationid", None))

	def assertKnownDevice(self, e, gatewayId = None):
		id = e.get("euid")
		self.assertDeviceIsComplete(e)

		if gatewayId is not None:
			self.assertEqual(gatewayId, e.get("gateid"))

		if id == "0xa335d00019f5234e":
			self.assertEqual("Temperature", e.get("name"))
			self.assertEqual("1", e.get("init"))
			self.assertEqual("unavailable", e.get("status"))
			self.assertEqual("0", e.get("type"))
		elif id == "0xa32d27aa5e94ecfd":
			self.assertEqual("Multi-sensor", e.get("name"))
			self.assertEqual("1", e.get("init"))
			self.assertEqual("unavailable", e.get("status"))
			self.assertEqual("3", e.get("type"))
		elif id == "0xa37e0f7f0302324d":
			self.assertEqual("Humidity", e.get("name"))
			self.assertEqual("available", e.get("status"))
			self.assertEqual("0", e.get("type"))
		elif id == "0xa371959aad24618e":
			self.assertEqual("Unknown", e.get("name"))
			self.assertEqual("0", e.get("init"))
			self.assertEqual("unavailable", e.get("status"))
			self.assertEqual("4", e.get("type"))
		else:
			self.assertTrue(False, "unknown device: %s, %s, %s" % (e.tag, e.attrib, e.text))

	def assertKnownModules(self, e):
		id = e.get("euid")

		if id == "0xa335d00019f5234e":
			self.assertEqual("Temperature", e.get("name"))
			self.assertEqual(5, len(e))

			for i in range(len(e)):
				self.assertEqual(str(i), e[i].get("id"))

			self.assertEqual("19", e[0].get("type"))
			self.assertEqual("true", e[0].get("valid"))
			self.assertEqual("19.5", e[0].get("value"))
			self.assertEqual("19", e[1].get("type"))
			self.assertEqual("true", e[1].get("valid"))
			self.assertEqual("0", e[1].get("value"))
			self.assertEqual("8", e[2].get("type"))
			self.assertEqual("true", e[2].get("valid"))
			# value for module 2 is random for each run
			self.assertEqual("2", e[3].get("type"))
			self.assertEqual("false", e[3].get("valid"))
			self.assertEqual("NaN", e[3].get("value"))
			self.assertEqual("16", e[4].get("type"))
			self.assertEqual("false", e[4].get("valid"))
			self.assertEqual("NaN", e[4].get("value"))

		elif id == "0xa32d27aa5e94ecfd":
			self.assertEqual("Multi-sensor", e.get("name"))

			for i in range(len(e)):
				self.assertEqual(str(i), e[i].get("id"))

			self.assertEqual("19", e[0].get("type"))
			self.assertEqual("true", e[0].get("valid"))
			self.assertEqual("20", e[0].get("value"))
			self.assertEqual("8", e[1].get("type"))
			self.assertEqual("false", e[1].get("valid"))
			self.assertEqual("NaN", e[1].get("value"))
			self.assertEqual("11", e[2].get("type"))
			self.assertEqual("false", e[2].get("valid"))
			self.assertEqual("NaN", e[2].get("value"))

		elif id == "0xa37e0f7f0302324d":
			self.assertEqual("Humidity", e.get("name"))
			self.assertEqual(5, len(e))

			for i in range(len(e)):
				self.assertEqual(str(i), e[i].get("id"))

			self.assertEqual("19", e[0].get("type"))
			self.assertEqual("false", e[0].get("valid"))
			self.assertEqual("NaN", e[0].get("value"))
			self.assertEqual("19", e[1].get("type"))
			self.assertEqual("false", e[1].get("valid"))
			self.assertEqual("NaN", e[1].get("value"))
			self.assertEqual("8", e[2].get("type"))
			self.assertEqual("false", e[2].get("valid"))
			self.assertEqual("NaN", e[2].get("value"))
			self.assertEqual("2", e[3].get("type"))
			self.assertEqual("false", e[3].get("valid"))
			self.assertEqual("NaN", e[3].get("value"))
			self.assertEqual("16", e[4].get("type"))
			self.assertEqual("false", e[4].get("valid"))
			self.assertEqual("NaN", e[4].get("value"))

		elif id == "0xa371959aad24618e":
			self.assertEqual("Unknown", e.get("name"))
			self.assertEqual(10, len(e))

			for i in range(len(e)):
				self.assertEqual(str(i), e[i].get("id"))

			self.assertEqual("19", e[0].get("type"))
			self.assertEqual("8", e[1].get("type"))
			self.assertEqual("11", e[2].get("type"))
			self.assertEqual("19", e[3].get("type"))
			self.assertEqual("8", e[4].get("type"))
			self.assertEqual("11", e[5].get("type"))
			self.assertEqual("19", e[6].get("type"))
			self.assertEqual("8", e[7].get("type"))
			self.assertEqual("11", e[8].get("type"))
			self.assertEqual("4", e[9].get("type"))

		else:
			self.assertTrue(False, "unknown device: %s, %s, %s" % (e.tag, e.attrib, e.text))

	"""
	Get all active devices. There should be 3 devices already
	prepared for testing. The humidity devices is expected to
	be available at the time of running this test.
	"""
	def test1_getall(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(DeviceGetAll(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(3, len(response.root))

		names = ["Temperature", "Humidity", "Multi-sensor"]
		for name in names:
			found = False
			for e in response.root:
				if e.get("name") == name:
					found = True
					break

			self.assertTrue(found, "missing: " + name)

		for e in response.root:
			self.assertKnownDevice(e, config.gateway_id)
			self.assertKnownModules(e)

	"""
	Get all inactive devices, i.e. devices discovered by a gateway.
	Such devices can be optionally activated. There should be one
	inactive device available for testing.
	"""
	def test2_getnew(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(DeviceGetNew(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		e = response.root[0]

		self.assertEqual("Unknown", e.get("name"))
		self.assertKnownDevice(e, config.gateway_id)
		self.assertKnownModules(e)

	"""
	Ask for devices with invalid EUID. Parsing should fail for
	all of them.
	"""
	def test3_get_by_invalid_euid(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			"a1231r30430"
		))
		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			"120342342",
			"--2323-131e3",
			"+__\;dew23121"
		))
		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

	"""
	Ask for a device with too big EUID (oveflows 64-bit unsigned integer).
	"""
	def test4_get_by_toobig_euid(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			"1208925819614629174706175"
		))
		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

	"""
	Ask for non-existing devices by a valid EUID (parser would
	not fail and the actual processing of the request should occur).
	"""
	def test5_get_non_existing(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			"102434131"
		))
		self.assertTrue(response.is_data())
		self.assertEqual(0, len(response.root))

		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			"102434131",
			"109092302001011321"
		))
		self.assertTrue(response.is_data())
		self.assertEqual(0, len(response.root))

	"""
	Test method get to obtain the inactive device named "Unknown".
	"""
	def test6_get_inactive(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(DeviceGetNew(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(1, len(response.root))

		euid = response.root[0].get("euid")

		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			euid
		))
		self.assertTrue(response.is_data)
		self.assertEqual(1, len(response.root))

		e = response.root[0]

		self.assertEqual(euid, e.get("euid"))
		self.assertEqual("Unknown", e.get("name"))
		self.assertKnownDevice(e, config.gateway_id)
		self.assertKnownModules(e)

	"""
	Test we can get many devices selectively by get command
	in any order.
	"""
	def test7_get_selective(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(DeviceGetAll(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(3, len(response.root))

		euids = []
		for e in response.root:
			euids.append(e.get("euid"))

		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			euids[2],
			euids[0],
			euids[1]
		))
		self.assertTrue(response.is_data())
		self.assertEqual(3, len(response.root))

		self.assertEqual(euids[2], response.root[0].get("euid"))
		self.assertEqual(euids[0], response.root[1].get("euid"))
		self.assertEqual(euids[1], response.root[2].get("euid"))

		for i in range(3):
			self.assertKnownDevice(response.root[i], config.gateway_id)
			self.assertKnownModules(response.root[i])

	"""
	Test we can get many devices selectively by get command where
	some devices do not exist or are duplicated.
	"""
	def test8_get_mixed(self):
		c = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

		response = c.request(DeviceGetAll(
			config.gateway_id,
			self.session
		))
		self.assertTrue(response.is_data())
		self.assertEqual(3, len(response.root))

		euids = []
		for e in response.root:
			euids.append(e.get("euid"))

		response = c.request(DeviceGet(
			config.gateway_id,
			self.session,
			euids[2],
			"944035231",
			euids[1],
			"1092091091",
			euids[1]
		))
		self.assertTrue(response.is_data())
		self.assertEqual(3, len(response.root))

		self.assertEqual(euids[2], response.root[0].get("euid"))
		self.assertEqual(euids[1], response.root[1].get("euid"))
		self.assertEqual(euids[1], response.root[2].get("euid"))

		for i in range(3):
			self.assertKnownDevice(response.root[i], config.gateway_id)
			self.assertKnownModules(response.root[i])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
