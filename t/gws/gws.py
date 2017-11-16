import websocket

def assureNotClosed(self, ws, timeout = 1):
	ws.timeout = timeout
	with self.assertRaises(websocket.WebSocketTimeoutException):
		ws.recv()

def assureIsClosed(self, ws):
	with self.assertRaises(websocket.WebSocketConnectionClosedException):
		ws.recv()
