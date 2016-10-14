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

	def sessionid(self):
		return self.root.get("sessionid")

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

class Logout(Request):
	def __init__(self, sessionid):
		Request.__init__(self, ns = "accounts",
				type = "logout", sessionid = sessionid)

class GatewayRegister(Request):
	def __init__(self, gateway, sessionid, **kwargs):
		Request.__init__(self, ns = "gates",
			type = "register", sessionid = sessionid)
		self.gateway = gateway

		if "name" in kwargs:
			self.name = kwargs["name"]
	
	def xml(self):
		request = Request.xml(self)
		gate = xml.SubElement(request, "gate")
		gate.set("id", self.gateway)

		if hasattr(self, "name"):
			gate.set("name", self.name)

		return request

class GatewayUnregister(Request):
	def __init__(self, gateway, sessionid):
		Request.__init__(self, ns = "gates",
			type = "unregister", sessionid = sessionid)
		self.gateway = gateway

	def xml(self):
		request = Request.xml(self)
		gate = xml.SubElement(request, "gate")
		gate.set("id", self.gateway)
		return request

class GatewayGet(Request):
	def __init__(self, gateway, sessionid):
		Request.__init__(self, ns = "gates",
			type = "get", sessionid = sessionid)
		self.gateway = gateway

	def xml(self):
		request = Request.xml(self)
		gate = xml.SubElement(request, "gate")
		gate.set("id", self.gateway)
		return request

class GatewayUpdate(Request):
	def __init__(self, gateway, sessionid, **kwargs):
		Request.__init__(self, ns = "gates",
			type = "update", sessionid = sessionid)
		self.gateway = gateway

		if "name" in kwargs:
			self.name = kwargs["name"]
		if "altitude" in kwargs:
			self.altitude = kwargs["altitude"]
		if "latitude" in kwargs:
			self.latitude = kwargs["latitude"]
		if "longitude" in kwargs:
			self.longitude = kwargs["longitude"]

	def xml(self):
		request = Request.xml(self)
		gate = xml.SubElement(request, "gate")
		gate.set("id", self.gateway)

		if hasattr(self, "name"):
			gate.set("name", self.name)
		if hasattr(self, "altitude"):
			gate.set("altitude", str(self.altitude))
		if hasattr(self, "latitude"):
			gate.set("latitude", str(self.latitude))
		if hasattr(self, "longitude"):
			gate.set("longitude", str(self.longitude))

		return request

class GatewayGetAll(Request):
	def __init__(self, sessionid):
		Request.__init__(self, ns = "gates",
			type = "getall", sessionid = sessionid)

	def xml(self):
		request = Request.xml(self)
		return request

class LocationAdd(Request):
	def __init__(self, gateway, sessionid, **kwargs):
		Request.__init__(self, ns = "locations",
			type = "add", sessionid = sessionid)
		self.gateway = gateway

		if "name" in kwargs:
			self.name = kwargs["name"]

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)
		location = xml.SubElement(request, "location")

		if hasattr(self, "name"):
			location.set("name", self.name)

		return request

class LocationDelete(Request):
	def __init__(self, gateway, location, sessionid):
		Request.__init__(self, ns = "locations",
			type = "delete", sessionid = sessionid)
		self.location = location
		self.gateway = gateway

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)
		location = xml.SubElement(request, "location")
		location.set("locationid", self.location)
		return request

class LocationGetAll(Request):
	def __init__(self, gateway, sessionid, **kwargs):
		Request.__init__(self, ns = "locations",
			type = "getall", sessionid = sessionid)
		self.gateway = gateway

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)

		return request

class LocationUpdate(Request):
	def __init__(self, gateway, location, sessionid, **kwargs):
		Request.__init__(self, ns = "locations",
			type = "update", sessionid = sessionid)
		self.location = location
		self.gateway = gateway

		if "name" in kwargs:
			self.name = kwargs["name"]

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)
		location = xml.SubElement(request, "location")
		location.set("locationid", self.location)

		if hasattr(self, "name"):
			location.set("name", self.name)

		return request
