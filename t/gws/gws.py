import websocket
import json

def assureNotClosed(self, ws, timeout = 1):
	ws.timeout = timeout
	with self.assertRaises(websocket.WebSocketTimeoutException):
		ws.recv()

def assureIsClosed(self, ws):
	with self.assertRaises(websocket.WebSocketConnectionClosedException):
		ws.recv()

def registerGateway(self, ws, gatewayID):
	msg = json.dumps(
		{
			"gateway_id" : gatewayID,
			"ip_address" : "192.168.1.1",
			"message_type" : "gateway_register",
			"version" : "v1.0"
		}
	)

	ws.send(msg)
	msg = json.loads(ws.recv())

	self.assertEqual("gateway_accepted", msg["message_type"])
	assureNotClosed(self, ws)
