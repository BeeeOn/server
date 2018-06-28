import websocket
import json
import zmq
import collections
import threading

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

class ZMQConnection:
	def __init__(self, endpoint):
		self._endpoint = endpoint
		self._ctx = zmq.Context()
		self._data = collections.deque()
		self._lock = threading.Lock()
		self._active = threading.Event()
		self._finish = threading.Event()
		self._stop = False

	def open(self):
		self._sock = self._ctx.socket(zmq.PULL)
		self._sock.setsockopt(zmq.LINGER, 0) # avoid 'Address already in use'
		self._sock.bind(self._endpoint)
		self.start()

	def close(self):
		self.stop()
		self._sock.close()

	def __enter__(self):
		self.open()

	def __exit__(self, type, value, traceback):
		self.close()

	def should_stop(self):
		with self._lock:
			return self._stop

	def stop(self, timeout = 5):
		with self._lock:
			self._stop = True

		self._finish.wait(timeout)

	def push_data(self, data):
		with self._lock:
			self._data.append(data)

		self._active.set()

	def _pop_data(self):
		with self._lock:
			return self._data.popleft()

	def pop_data(self, timeout = 5):
		if timeout == 0:
			return self._pop_data()
		else:
			self.wait_for_data(timeout)
			return self._pop_data()

	def has_data(self):
		with self._lock:
			return len(self._data) > 0

	def wait_for_data(self, timeout = 5):
		self._active.wait(timeout)
		self._active.clear()
		return self.has_data()

	def start(self):
		self._thread = threading.Thread(target = self.run)
		self._thread.start()

	def run(self):
		self._run()
		self._finish.set()

	def _run(self):
		poller = zmq.Poller()
		poller.register(self._sock, zmq.POLLIN)

		while not self.should_stop():
			self._do_poll(poller)

	def _do_poll(self, poller):
		for sock, event in dict(poller.poll(100)).items():
			if event != zmq.POLLIN:
				continue

			try:
				msg = sock.recv_json(zmq.NOBLOCK)
				self.push_data(msg)
			except Exception as e:
				self.push_data(e)
