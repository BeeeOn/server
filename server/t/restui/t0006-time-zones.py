#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import json
import base64

from rest import GET

class TestTimeZones(unittest.TestCase):
	def fetch_zone_by_id(self, id):
		req = GET(config.ui_host, config.ui_port, "/time/zones/" + id)
		response, content = req()

		self.assertEqual(200, response.status)

		data = json.loads(content)
		self.assertEqual("success", data["status"])

		return data["data"]

	def fetch_zones(self):
		req = GET(config.ui_host, config.ui_port, "/time/zones")
		response, content = req()

		self.assertEqual(200, response.status)

		data = json.loads(content)
		self.assertEqual("success", data["status"])

		zones = data["data"]
		self.assertTrue(len(zones) > 0)

		return zones

	"""
	Assure at least one time-zone is returned. Because, time-zones can change
	over time based on the system installation.
	"""
	def test1_list_time_zones(self):
		zones = self.fetch_zones()

		no_dups = {}
		for zone in zones:
			id = zone["id"]

			self.assertFalse(id in no_dups, "duplicate zone %s" % id)
			no_dups[id] = 1

		for zone in zones:
			id = zone["id"]
			self.assertTrue("short_name" in zone, "short_name is missing for %s" % id)
			self.assertTrue("display_name" in zone, "display_name is missing for %s" % id)
			self.assertTrue("utc_offset" in zone, "utc_offset is missing for %s" % id)
			self.assertTrue("dst_offset" in zone, "dst_offset is missing for %s" % id)
			self.assertTrue("dst_in_effect" in zone, "dst_in_effect is missing for %s" % id)

	"""
	Test information about zone Europe/Prague are correct. The test depends on time.
	When DST is in effect, the result is different. This means that during the first
	half of the year, we test only one variant and during the second we test the opposite.
	Client has no ability to tell server, what is the reference time for the expected results.
	"""
	def test2_europe_prague(self):
		zone = self.fetch_zone_by_id("Europe/Prague")

		self.assertEqual("Europe/Prague", zone["id"])
		self.assertEqual(3600, zone["utc_offset"])
		self.assertEqual(3600, zone["dst_offset"])

		if zone["dst_in_effect"]:
			self.assertEqual("GMT+02:00", zone["short_name"])
			self.assertEqual("Europe/Prague (GMT+02:00)", zone["display_name"])
		else:
			self.assertEqual("GMT+01:00", zone["short_name"])
			self.assertEqual("Europe/Prague (GMT+01:00)", zone["display_name"])

	"""
	Test information about zone Europe/London are correct. The test depends on time.
	When DST is in effect, the result is different. This means that during the first
	half of the year, we test only one variant and during the second we test the opposite.
	Client has no ability to tell server, what is the reference time for the expected results.
	"""
	def test3_europe_london(self):
		zone = self.fetch_zone_by_id("Europe/London")

		self.assertEqual("Europe/London", zone["id"])
		self.assertEqual(0, zone["utc_offset"])
		self.assertEqual(3600, zone["dst_offset"])

		if zone["dst_in_effect"]:
			self.assertEqual("GMT+01:00", zone["short_name"])
			self.assertEqual("Europe/London (GMT+01:00)", zone["display_name"])
		else:
			self.assertEqual("GMT", zone["short_name"])
			self.assertEqual("Europe/London (GMT)", zone["display_name"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
