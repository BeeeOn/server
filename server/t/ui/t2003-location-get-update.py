#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
import uuid
import copy

from rest import POST, PUT, DELETE, GET

class TestLocation(unittest.TestCase):
	"""
	Prepare a session, a testing place 'My Home' and a testing
	location 'My Livingroom'.
	"""
	def setUp(self):
		print("# setup")

		req = POST(config.ui_host, config.ui_port, "/auth")
		req.body(config.PERMIT_LOGIN)
		response, content = req()

		self.assertEqual(200, response.status)
		self.session = content
		print("# got session: " + self.session)

		place = json.dumps({"name": "My Home"})
		req = POST(config.ui_host, config.ui_port, "/place")
		req.authorize(self.session)
		req.body(place)
		response, content = req()

		self.assertEqual(200, response.status)
		self.place = json.loads(content)
		print("# got place: " + self.place["id"])

		place = json.dumps({"name": "My Second Home"})
		req = POST(config.ui_host, config.ui_port, "/place")
		req.authorize(self.session)
		req.body(place)
		response, content = req()

		self.assertEqual(200, response.status)
		self.second_place = json.loads(content)
		print("# got second place: " + self.second_place["id"])

		location = json.dumps({"name": "My Livingroom"})
		req = POST(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] + "/locations")
		req.authorize(self.session)
		req.body(location)
		response, content = req()

		self.assertEqual(200, response.status)
		self.location = json.loads(content)
		print("# got location: " + self.location["id"])

	"""
	Destroy the location 'My Livingroom', the place 'My Home'
	and destroy the session.
	"""
	def tearDown(self):
		print("# teardown")

		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" + self.location["id"])
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(200, response.status)

		req = DELETE(config.ui_host, config.ui_port,
				"/place/" + self.place["id"])
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(200, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(200, response.status)

	"""
	Request a non-existing location.
	"""
	def test1_get_non_existing(self):
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/any-id")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(400, response.status)
		self.assertEqual(0, len(content))

	"""
	Request a non-existing location (by valid UUID).
	"""
	def test2_get_non_existing_uuid(self):
		id = str(uuid.uuid4())

		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" +	id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)
		self.assertEqual(0, len(content))

	"""
	Request an existing location.
	"""
	def test3_get_existing(self):
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" + self.location["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		location = json.loads(content)
		self.assertEqual("My Livingroom", location["name"])
		self.assertEqual(self.location["id"], location["id"])

	"""
	Request an existing location from wrong place.
	We must not be successful here.
	"""
	def test4_get_existing_from_wrong_place(self):
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.second_place["id"] +
				"/locations/" + self.location["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)
		self.assertEqual(0, len(content))

	"""
	Update a non-existing location. Should return 404 and it must
	not create such location.
	"""
	def test5_update_non_existing(self):
		id = str(uuid.uuid4())

		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" + id)
		req.authorize(self.session)
		req.body(json.dumps({
			"name" : "Non-existing"
		}))
		response, content = req()

		self.assertEqual(404, response.status)
		self.assertEqual(0, len(content))

		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" + id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(404, response.status)
		self.assertEqual(0, len(content))

	"""
	Update the existing location 'My Livingroom' in place 'My Home'.
	"""
	def test6_update_existing(self):
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" + self.location["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		location = json.loads(content)
		self.assertEqual("My Livingroom", location["name"])
		self.assertEqual(self.location["id"], location["id"])

		self.assertNotEqual("XXXYYY", location["name"])
		location["name"] = "XXXYYY"

		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" + self.location["id"])
		req.authorize(self.session)
		req.body(json.dumps(location))
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("XXXYYY", result["name"])
		self.assertEqual(self.location["id"], result["id"])

	"""
	Update the existing location 'My Livingroom' but in the wrong
	place. Should return 404 and it must not update such location.
	"""
	def test7_update_existing_in_wrong_place(self):
		location = copy.copy(self.location)
		self.assertNotEqual("XXXYYY", location["name"])
		location["name"] = "XXXYYY"

		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.second_place["id"] +
				"/locations/" + self.location["id"])
		req.authorize(self.session)
		req.body(json.dumps(location))
		response, content = req()

		self.assertEqual(404, response.status)
		self.assertEqual(0, len(content))

		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"] +
				"/locations/" + self.location["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		location = json.loads(content)
		self.assertEqual("My Livingroom", location["name"])
		self.assertEqual(self.location["id"], location["id"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
