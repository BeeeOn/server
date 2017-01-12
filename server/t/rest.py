from http.client import HTTPConnection

class Rest:
	def __init__(self, host, port, uri, method):
		self.m_host = host
		self.m_port = port
		self.m_method = method
		self.m_uri = uri
		self.m_headers = {}
		self.m_body = None

	def __setitem__(self, key, value):
		self.m_headers[key] = value

	def __getitem__(self, key):
		return self.m_headers[key]

	def __delitem__(self, key):
		del self.m_headers[key]

	def body(self, body):
		self.m_body = body

	def authorize(self, sessionId):
		self.m_headers["Authorization"] = "Bearer " + sessionId

	def perform(self):
		conn = HTTPConnection(self.m_host, self.m_port, timeout = 10)
		conn.request(self.m_method, self.m_uri,
			headers = self.m_headers, body = self.m_body)
		response = conn.getresponse()
		content = str(response.read(), "utf-8")
		conn.close()
		return response, content

	def __call__(self):
		return self.perform()

class GET(Rest):
	def __init__(self, host, port, uri):
		Rest.__init__(self, host, port, uri, "GET")

class POST(Rest):
	def __init__(self, host, port, uri):
		Rest.__init__(self, host, port, uri, "POST")

class PUT(Rest):
	def __init__(self, host, port, uri):
		Rest.__init__(self, host, port, uri, "PUT")

class DELETE(Rest):
	def __init__(self, host, port, uri):
		Rest.__init__(self, host, port, uri, "DELETE")

class OPTIONS(Rest):
	def __init__(self, host, port, uri):
		Rest.__init__(self, host, port, uri, "OPTIONS")
