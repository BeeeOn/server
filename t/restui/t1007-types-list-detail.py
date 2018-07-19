#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import GET, POST, PUT, DELETE

class TestTypesListDetail(unittest.TestCase):
	"""
	Create a session for testing.
	"""
	def setUp(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		req.body(config.PERMIT_LOGIN)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.session = result["data"]["id"]

	def tearDown(self):
		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

	"""
	List all available types.
	"""
	def test1_list_all(self):
		req = GET(config.ui_host, config.ui_port, "/types")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual(27, len(result["data"]))

	def test2_detail_of_non_existing(self):
		req = GET(config.ui_host, config.ui_port, "/types/12904232")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)

		result = json.loads(content)
		self.assertEqual("error", result["status"])
		self.assertEqual("requested resource does not exist", result["message"])

	def test3_detail_of_battery(self):
		req = GET(config.ui_host, config.ui_port, "/types/battery")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.assertEqual("battery", result["data"]["name"])
		self.assertEqual("%", result["data"]["unit"])

	def assure_range(self, id, name, min, max, step):
		req = GET(config.ui_host, config.ui_port, "/types/" + id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		type = result["data"]
		self.assertEqual(name, type["name"])
		self.assertTrue("range" in type)

		if min is not None:
			self.assertTrue("min" in type["range"])
			self.assertEqual(min, type["range"]["min"])
		else:
			self.assertFalse("min" in type["range"])

		if max is not None:
			self.assertTrue("max" in type["range"])
			self.assertEqual(max, type["range"]["max"])
		else:
			self.assertFalse("max" in type["range"])

		if step is not None:
			self.assertTrue("step" in type["range"])
			self.assertEqual(step, type["range"]["step"])
		else:
			self.assertFalse("step" in type["range"])

	def test4_check_types_with_ranges(self):
		self.assure_range("battery", "battery", 0, 100, 1)
		self.assure_range("brightness", "brightness", 0, 100, 1)
		self.assure_range("co2", "CO2", 0, 1000000, 1)
		self.assure_range("humidity", "humidity", 0, 100, 1)
		self.assure_range("luminance", "luminance", 0, 1000000, 1)
		self.assure_range("noise", "noise", 0, 200, 1)
		self.assure_range("performance", "performance", 0, 100, 1)
		self.assure_range("pressure", "pressure", 800, 1100, 1)
		self.assure_range("rssi", "signal", 0, 100, 1)
		self.assure_range("temperature", "temperature", -273.15, 200, 0.01)
		self.assure_range("ultraviolet", "UV", 0, 11, 0.1)
		self.assure_range("color_temperature", "color_temperature", 1700, 27000, 1)
		self.assure_range("color", "color", 0, 16777215, 1)

	def assure_values(self, id, name, values):
		req = GET(config.ui_host, config.ui_port, "/types/" + id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		type = result["data"]
		self.assertEqual(name, type["name"])
		self.assertTrue("values" in type)
		self.assertEqual(len(values), len(type["values"]))

		for key in values:
			self.assertTrue(key in type["values"])
			self.assertEqual(values[key], type["values"][key])

	def test5_check_types_with_values(self):
		self.assure_values("availability", "availability", {"0": "unavailable", "1": "available"})
		self.assure_values("fire", "fire", {"0": "no fire", "1": "fire"})
		self.assure_values("motion", "motion", {"0": "no motion", "1": "motion"})
		self.assure_values("open_close", "open/close", {"0": "closed", "1": "open"})
		self.assure_values("on_off", "on/off", {"0": "off", "1": "on"})
		self.assure_values("security_alert", "security alert", {"0": "ease", "1": "alert"})
		self.assure_values("shake", "shake", {"0": "ease", "1": "shake"})

	def assure_levels(self, id, name, levels):
		req = GET(config.ui_host, config.ui_port, "/types/" + id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		type = result["data"]
		self.assertEqual(name, type["name"])
		self.assertTrue("levels" in type)
		self.assertEqual(len(levels), len(type["levels"]))

		for i in range(len(levels)):
			if levels[i][0] is None:
				self.assertFalse("min" in type["levels"][i])
			else:
				self.assertTrue("min" in type["levels"][i])
				self.assertEqual(levels[i][0], type["levels"][i]["min"])

			if levels[i][1] is None:
				self.assertFalse("max" in type["levels"][i])
			else:
				self.assertTrue("max" in type["levels"][i])
				self.assertEqual(levels[i][1], type["levels"][i]["max"])

			if levels[i][2] is None:
				self.assertFalse("attention" in type["levels"][i])
			else:
				self.assertTrue("attention" in type["levels"][i])
				self.assertEqual(levels[i][2], type["levels"][i]["attention"])

			if levels[i][3] is None:
				self.assertFalse("name" in type["levels"][i])
			else:
				self.assertTrue("name" in type["levels"][i])
				self.assertEqual(levels[i][3], type["levels"][i]["name"])

	def test6_check_types_with_levels(self):
		self.assure_levels("battery", "battery", [
			(0, 10, "single", "critical"),
			(11, 25, "single", "low"),
			(26, 80, None, "medium"),
			(81, 100, None, "high")
		])
		self.assure_levels("co2", "CO2", [
			(None, 450, None, "normal outdoor"),
			(451, 1000, None, "normal indoor"),
			(1001, 2500, None, "poor air"),
			(2501, 5000, "single", "adverse health effects"),
			(5001, 10000, "repeat", "dangerous after few hours"),
			(10001, 30000, "repeat" , "dangerous after several minutes"),
			(30001, None, "alert", "extreme and dangerous")
		])
		self.assure_levels("fire", "fire", [
			(1, 1, "alert", None)
		])
		self.assure_levels("motion", "motion", [
			(1, 1, "single", None)
		])
		self.assure_levels("noise", "noise", [
			(None, 80, None, "normal"),
			(81, 90, None, "acceptable"),
			(91, 99, "single", "loud"),
			(100, 111, "repeat", "dangerous for several minutes stay"),
			(112, 139, "repeat", "dangerous for few minutes stay"),
			(140, None, "alert", "immediate nerve damage possible"),
		])
		self.assure_levels("performance", "performance", [
			(0, 0, None, "idle"),
			(95, None, None, "high load")
		])
		self.assure_levels("rssi", "signal", [
			(None, 25, None, "poor"),
			(26, 80, None, "good"),
			(81, 100, None, "high")
		])
		self.assure_levels("security_alert", "security alert", [
			(1, 1, "alert", None)
		])
		self.assure_levels("ultraviolet", "UV", [
			(None, 2.9, None, "low"),
			(3, 5.9, None, "moderate"),
			(6, 7.9, "single", "high"),
			(8, 10.9, "single", "very high"),
			(11, None, "single", "extreme")
		])

	def test7_check_enums(self):
		req = GET(config.ui_host, config.ui_port, "/types/enum/MOD_BOILER_STATUS")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		enum = result["data"]
		self.assertEqual("boiler status", enum["name"])
		self.assertTrue("values" in enum)
		self.assertEqual(5, len(enum["values"]))

		self.assertEqual("undefined", enum["values"]["0"])
		self.assertEqual("heating", enum["values"]["1"])
		self.assertEqual("heating water", enum["values"]["2"])
		self.assertEqual("failure", enum["values"]["3"])
		self.assertEqual("shutdown", enum["values"]["4"])

	def test8_check_bitmap_with_flags(self):
		req = GET(config.ui_host, config.ui_port, "/types/bitmap/MOD_CURRENT_BOILER_OT_FAULT_FLAGS")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		bitmap = result["data"]
		self.assertEqual("OT Fault Flags", bitmap["name"])
		self.assertTrue("flags" in bitmap)
		self.assertEqual(6, len(bitmap["flags"]))

		self.assertEqual("service request", bitmap["flags"]["0"]["name"])
		self.assertEqual("lockout reset enabled", bitmap["flags"]["1"]["name"])
		self.assertEqual("low water pressure", bitmap["flags"]["2"]["name"])
		self.assertEqual("gas/flame fault", bitmap["flags"]["3"]["name"])
		self.assertEqual("air pressure fault", bitmap["flags"]["4"]["name"])
		self.assertEqual("water overheated", bitmap["flags"]["5"]["name"])

	def test9_check_bitmap_with_group(self):
		req = GET(config.ui_host, config.ui_port, "/types/bitmap/MOD_CURRENT_BOILER_OT_OEM_FAULTS")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		bitmap = result["data"]
		self.assertEqual("OT OEM Faults", bitmap["name"])
		self.assertTrue("groups" in bitmap)
		self.assertEqual(1, len(bitmap["groups"]))

		self.assertEqual("OEM specific", bitmap["groups"][0]["name"])
		self.assertEqual(8, len(bitmap["groups"][0]["mapping"]))
		self.assertEqual(0, bitmap["groups"][0]["mapping"][0])
		self.assertEqual(1, bitmap["groups"][0]["mapping"][1])
		self.assertEqual(2, bitmap["groups"][0]["mapping"][2])
		self.assertEqual(3, bitmap["groups"][0]["mapping"][3])
		self.assertEqual(4, bitmap["groups"][0]["mapping"][4])
		self.assertEqual(5, bitmap["groups"][0]["mapping"][5])
		self.assertEqual(6, bitmap["groups"][0]["mapping"][6])
		self.assertEqual(7, bitmap["groups"][0]["mapping"][7])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
