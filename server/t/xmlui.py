import xml.etree.ElementTree as xml
import socket

class Request:
	def __init__(self, version = "1.0.0", **kwargs):
		self.version = version

		if "type" in kwargs:
			self.type = kwargs["type"]
		if "ns" in kwargs:
			self.ns = kwargs["ns"]
		if "sessionid" in kwargs:
			self.sessionid = kwargs["sessionid"]

	def xml(self):
		request = xml.Element("request")
		request.set("version", self.version)

		if hasattr(self, "type"):
			request.set("type", self.type)
		if hasattr(self, "ns"):
			request.set("ns", self.ns)
		if hasattr(self, "sessionid"):
			request.set("sessionid", self.sessionid)

		return request

class Response:
	def __init__(self, root):
		if root.tag != "response":
			raise Exception("unexpected tag: " + root.tag)
		self.root = root

	def version(self):
		return self.root.get("version")

	def ns(self):
		return self.root.get("ns")

	def is_ok(self):
		return self.root.get("result") == "ok"

	def is_data(self):
		return self.root.get("result") == "data"

	def is_error(self):
		return self.root.get("result") == "error"

	def error_code(self):
		return self.root.get("errcode")

	def __str__(self):
		return str(xml.tostring(self.root), "utf-8")

class Connector:
	def __init__(self, host, port):
		self.host = host
		self.port = port

	def request(self, request, maxlen = 1024):
		s = socket.socket()
		s.connect((self.host, self.port))
		s.settimeout(1)

		if isinstance(request, Request):
			root = request.xml()
		elif isinstance(request, xml.Element):
			root = request
		else:
			raise Exception("unexpected request: " + str(request))

		s.sendall(xml.tostring(root))
		raw = s.recv(maxlen)
		s.close()

		result = xml.fromstring(raw)
		return Response(result)

class Login(Request):
	def __init__(self, provider, authCode):
		Request.__init__(self, ns = "accounts", type = "login")
		self.provider = provider
		self.authCode = authCode

	def xml(self):
		request = Request.xml(self)
		provider = xml.SubElement(request, "provider")
		provider.set("name", self.provider)
		provider.set("authCode", self.authCode)
		return request
