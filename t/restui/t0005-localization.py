#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import json

from rest import POST, GET, DELETE

class TestLocalization(unittest.TestCase):
	def test1_error_cs(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		req["Accept-Language"] = "cs-CZ"
		response, content = req()

		self.assertEqual(401, response.status)
		result = json.loads(content)
		self.assertEqual("error", result["status"])
		self.assertEqual(401, result["code"])
		self.assertEqual("uživatel nepřihlášen", result["message"])

class TestLocalizationAfterLogin(unittest.TestCase):
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
	Test that the language used by the server is determined from session or
	from Accept-Language, however, to prefer the Accept-Language over the
	session language, the header "X-Prefer-Request-Language: yes" must be given.
	"""
	def test1_type_availability_multilang(self):
		# Session language is used
		req = GET(config.ui_host, config.ui_port, "/types/1")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual("availability", result["data"]["name"])

		# Accept-Language does NOT override session language
		req = GET(config.ui_host, config.ui_port, "/types/1")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual("availability", result["data"]["name"])

		# Accept-Language DOES override session language
		req = GET(config.ui_host, config.ui_port, "/types/1")
		req.authorize(self.session)
		req["Accept-Language"] = "cs-CZ"
		req["X-Prefer-Request-Language"] = "yes"
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual("přítomnost", result["data"]["name"])

	"""
	Test locale of the user we are logged in as (joe.doe@example.org).
	"""
	def test2_current_locale_is_en(self):
		req = GET(config.ui_host, config.ui_port, "/profiles/current")
		req.authorize(self.session)
		response, _ = req()

		self.assertEqual(303, response.status)

		req = GET(config.ui_host, config.ui_port, response.headers["Location"])
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		result = json.loads(content)
		self.assertEqual("success", result["status"])

		self.assertEqual("en", result["data"]["locale"])

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
