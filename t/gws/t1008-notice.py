#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json
import uuid

from rest import GET, POST, PUT, DELETE
from gws import assureIsClosed, assureNotClosed, registerGateway, ZMQConnection

class TestNotice(unittest.TestCase):
	def setUp(self):
		req = POST(config.ui_host, config.ui_port, "/auth")
		req.body(config.PERMIT_LOGIN)
		response, content = req()

		self.assertEqual(200, response.status)

		result = json.loads(content)
		self.assertEqual("success", result["status"])
		self.session = result["data"]["id"]

		req = POST(config.ui_host, config.ui_port, "/gateways")
		req.authorize(self.session)
		req.body(json.dumps(
			{"id": config.gateway_id, "name": "Testing Gateway", "timezone_id": "Europe/Prague"}
		))
		response, _ = req()

		self.assertEqual(201, response.status)

		self.ws = websocket.WebSocket()
		self.ws.connect(config.gws_ws_uri)
		registerGateway(self, self.ws, config.gateway_id)

	def tearDown(self):
		self.ws.close()

		req = DELETE(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id)
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

		req = DELETE(config.ui_host, config.ui_port, "/auth")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(204, response.status)

	def test1_notice_valid(self):
		id = str(uuid.uuid4())

		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/messages")
		req.authorize(self.session)
		response, content = req()
		self.assertEqual(200, response.status)
		messages_count = len(json.loads(content)["data"])

		msg = json.dumps(
			{
				"id": id,
				"message_type": "notice_request",
				"at": 1546363722908213,
				"severity": "error",
				"key": "pair_device_failed",
				"context": {
					"gateway_id": config.gateway_id,
					"device_id": "0xa701020311223344"
				}
			}
		)

		self.ws.send(msg)
		response = json.loads(self.ws.recv())

		self.assertEqual("generic_response", response["message_type"])
		self.assertEqual(id, response["id"])
		assureNotClosed(self, self.ws)

		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/messages")
		req.authorize(self.session)
		response, content = req()
		self.assertEqual(200, response.status)
		messages = json.loads(content)["data"]
		self.assertEqual(messages_count + 1, len(messages))
		self.assertEqual(1546363722908, messages[0]["at"])
		self.assertEqual("error", messages[0]["severity"])
		self.assertEqual("gateway.pair_device_failed", messages[0]["key"])
		self.assertEqual(config.gateway_id, messages[0]["context"]["gateway_id"])
		self.assertEqual("0xa701020311223344", messages[0]["context"]["device_id"])

	def test2_notice_with_custom_key(self):
		id = str(uuid.uuid4())

		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/messages")
		req.authorize(self.session)
		response, content = req()
		self.assertEqual(200, response.status)
		messages_count = len(json.loads(content)["data"])

		msg = json.dumps(
			{
				"id": id,
				"message_type": "notice_request",
				"at": 1546363722909123,
				"severity": "info",
				"key": "some_message",
				"context": {
					"reason": "unknown",
					"attempt": 1,
					"valid": True
				}
			}
		)

		self.ws.send(msg)
		response = json.loads(self.ws.recv())

		self.assertEqual("generic_response", response["message_type"])
		self.assertEqual(id, response["id"])
		assureNotClosed(self, self.ws)

		req = GET(config.ui_host, config.ui_port, "/gateways/" + config.gateway_id + "/messages")
		req.authorize(self.session)
		response, content = req()

		self.assertEqual(200, response.status)
		messages = json.loads(content)["data"]
		self.assertEqual(messages_count + 1, len(messages))

		message = None
		for m in messages:
			if m["at"] == 1546363722909:
				message = m

		self.assertIsNotNone(message)
		self.assertEqual("info", message["severity"])
		self.assertEqual("gateway.some_message", message["key"])
		self.assertEqual("unknown", message["context"]["reason"])
		self.assertEqual(1, message["context"]["attempt"])
		self.assertEqual(True, message["context"]["valid"])

	def test3_notice_key_invalid(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"id": id,
				"message_type": "notice_request",
				"at": 1546363722908213,
				"severity": "info",
				"key": "<script>alert('attack')</script>"
			}
		)

		self.ws.send(msg)
		assureIsClosed(self, self.ws)

	def test4_notice_context_key_invalid(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"id": id,
				"message_type": "notice_request",
				"at": 1546363722908213,
				"severity": "info",
				"key": "some_message",
				"context": {
					"<script>alert('attack')</script>": "x"
				}
			}
		)

		self.ws.send(msg)
		assureIsClosed(self, self.ws)

	def test5_notice_context_value_invalid(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"id": id,
				"message_type": "notice_request",
				"at": 1546363722908213,
				"severity": "info",
				"key": "some_message",
				"context": {
					"id": "<script>alert('attack')</script>"
				}
			}
		)

		self.ws.send(msg)
		assureIsClosed(self, self.ws)

	def test6_notice_context_too_deep(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"id": id,
				"message_type": "notice_request",
				"at": 1546363722908213,
				"severity": "info",
				"key": "some_message",
				"context": {
					"gateway": {
						"id": config.gateway_id
					}
				}
			}
		)

		self.ws.send(msg)
		assureIsClosed(self, self.ws)

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
