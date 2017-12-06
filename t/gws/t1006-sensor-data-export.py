#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import websocket
import json
import time
import uuid

from gws import assureIsClosed, assureNotClosed, registerGateway

class TestSensorData(unittest.TestCase):
	def setUp(self):
		self.ws = websocket.WebSocket()
		self.ws.connect(config.gws_ws_uri)

		registerGateway(self, self.ws, config.gateway_id)

	def tearDown(self):
		self.ws.close()

	"""
	Server just confirms that it received valid sensor data message,
	but nothing more can be determined from its response
	"""
	def test1_export_successful(self):
		id = str(uuid.uuid4())
		timestamp = time.time()

		msg = json.dumps(
			{
				"message_type" : "sensor_data_export",
				"id" : id,
				"data" : [
					{
						"device_id" : "0xa32d27aa5e94ecfd",
						"timestamp" : timestamp,
						"values": [
							{
								"module_id" : "0",
								"value" : 30.0,
								"valid" : "true"
							},
							{
								"module_id" : "1",
								"valid" : "false"
							},
							{
								"module_id" : "2",
								"value" : 60.0,
								"valid" : "true"
							}
						]
					}
				]
			}
		)

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("sensor_data_confirm", msg["message_type"])
		self.assertEqual(id, msg["id"])
		assureNotClosed(self, self.ws)

	"""
	Even if we send an invalid export message, we get just "confirm" response.
	This test is semi-automatic, it requires to check the server log.
	"""
	def test1_export_fails_due_to_unexisting_device(self):
		id = str(uuid.uuid4())

		msg = json.dumps(
			{
				"message_type" : "sensor_data_export",
				"id" : id,
				"data" : [
					{
						"device_id" : "0xa32d275555555555",
						"timestamp" : 0,
						"values": [
							{
								"module_id" : "5",
								"value" : -1230.0,
								"valid" : "true"
							},
							{
								"module_id" : "22",
								"valid" : "false"
							},
							{
								"module_id" : "89",
								"value" : 3460.132,
								"valid" : "true"
							}
						]
					}
				]
			}
		)

		self.ws.send(msg)
		msg = json.loads(self.ws.recv())

		self.assertEqual("sensor_data_confirm", msg["message_type"])
		self.assertEqual(id, msg["id"])
		assureNotClosed(self, self.ws)


if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
