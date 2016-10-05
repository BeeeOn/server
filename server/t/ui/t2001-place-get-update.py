#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json
import uuid

from rest import POST, PUT, DELETE, GET

class TestPlace(unittest.TestCase):
	"""
	Create a session and a testing place 'My Home'.
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

	"""
	Delete the place 'My Home' and destroy the session.
	"""
	def tearDown(self):
		print("# teardown")

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
	Request a non-existing place.
	"""
	def test1_get_non_existing(self):
		req = GET(config.ui_host, config.ui_port, "/place/any-id")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(400, response.status)
		self.assertEqual(0, len(content))

	def test2_get_non_existing_uuid(self):
		id = str(uuid.uuid4())
		req = GET(config.ui_host, config.ui_port, "/place/" + id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(403, response.status)
		self.assertEqual(0, len(content))

	"""
	Request an existing place.
	"""
	def test3_get_existing(self):
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		data = json.loads(content)
		self.assertEqual(self.place["name"], data["name"])
		self.assertIn(self.place["id"], data["id"])

	"""
	Update a non-existing place. Should return 404 and it must
	not create such place.
	"""
	def test4_update_non_existing_place(self):
		id = str(uuid.uuid4())

		req = PUT(config.ui_host, config.ui_port, "/place/" + id)

		data = {}
		data["name"] = "Non-existing"

		req.body(json.dumps(data))
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(403, response.status)
		self.assertEqual(0, len(content))

		req = GET(config.ui_host, config.ui_port, "/place/" + id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(403, response.status)
		self.assertEqual(0, len(content))

	"""
	Test a regular place update.
	"""
	def test5_update_place(self):
		req = GET(config.ui_host, config.ui_port,
				"/place/" + self.place["id"])
		req.authorize(self.session)
		response, content = req()

		data = json.loads(content)

		self.assertEqual(200, response.status)
		self.assertEqual(self.place["name"], data["name"])
		self.assertIn(self.place["id"], data["id"])

		self.assertNotEqual("XXXYYY", data["name"])
		data["name"] = "XXXYYY"

		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.place["id"])
		req.body(json.dumps(data))
		req.authorize(self.session)
		response, content = req()

		result = json.loads(content)

		self.assertEqual(self.place["id"], result["id"])
		self.assertEqual("XXXYYY", result["name"])

	"""
	Test update is performed on the place defined by ID in URI
	and not in the JSON body.
	"""
	def test6_update_place_wrong_id(self):
		place = json.dumps({"name": "My Second Home"})
		req = POST(config.ui_host, config.ui_port, "/place")
		req.authorize(self.session)
		req.body(place)
		response, content = req()

		secondHome = json.loads(content)

		self.assertEqual(200, response.status)
		self.assertTrue(len(secondHome["id"]) > 0)
		self.assertIsNotNone(uuid.UUID(secondHome["id"]))
		self.assertNotEqual(self.place["id"], secondHome["id"])

		req = PUT(config.ui_host, config.ui_port,
				"/place/" + self.place["id"])
		req.body(json.dumps(secondHome))
		req.authorize(self.session)
		response, content = req()

		result = json.loads(content)

		self.assertEqual(self.place["id"], result["id"])
		self.assertNotEqual(self.place["id"], secondHome["id"])
		self.assertEqual("My Second Home", result["name"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
