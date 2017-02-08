import xml.etree.ElementTree as xml
import time
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
	def __init__(self, root, expectType = None):
		if root.tag != "response":
			raise Exception("unexpected tag: " + root.tag)
		self.root = root

		if expectType is None:
			return

		if root.get("type") != expectType:
			raise Exception("unexpected response type: %s" %
					(root.get("type")))

	def version(self):
		return self.root.get("version")

	def ns(self):
		return self.root.get("ns")

	def sessionid(self):
		if self.root[0].tag != "session":
			raise Exception("no 'session' tag: " +
					self.root[0].tag)

		return self.root[0].get("id")

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
		s.settimeout(10)

		if isinstance(request, Request):
			root = request.xml()
		elif isinstance(request, xml.Element):
			root = request
		else:
			raise Exception("unexpected request: " + str(request))

		s.sendall(xml.tostring(root))
		raw = b""

		while b"</response>" not in raw:
			data = s.recv(maxlen)
			if len(data) == 0:
				break
			raw += data

		s.close()

		result = xml.fromstring(raw)
		if hasattr(request, "type"):
			return Response(result, request.type)
		else:
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
			type = "create", sessionid = sessionid)
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

class GetMyProfile(Request):
	def __init__(self, sessionid, **kwargs):
		Request.__init__(self, ns = "accounts",
			type = "getmyprofile", sessionid = sessionid)

	def xml(self):
		request = Request.xml(self)
		return request

class DeviceGetAll(Request):
	def __init__(self, gateway, sessionid, **kwargs):
		Request.__init__(self, ns = "devices",
			type = "getall", sessionid = sessionid)
		self.gateway = gateway

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)

		return request

class DeviceGetNew(Request):
	def __init__(self, gateway, sessionid, **kwargs):
		Request.__init__(self, ns = "devices",
			type = "getnew", sessionid = sessionid)
		self.gateway = gateway

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)

		return request

class DeviceGet(Request):
	def __init__(self, gateway, sessionid, *devices, **kwargs):
		Request.__init__(self, ns = "devices",
			type = "get", sessionid = sessionid)
		self.gateway = gateway
		self.devices = devices

	def xml(self):
		request = Request.xml(self)

		for device in self.devices:
			e = xml.SubElement(request, "device")
			e.set("gateid", self.gateway)
			e.set("euid", device)

		return request

class DeviceUpdate(Request):
	def __init__(self, gateway, device, sessionid, **kwargs):
		Request.__init__(self, ns = "devices",
			type = "update", sessionid = sessionid)
		self.gateway = gateway
		self.device = device

		if "name" in kwargs:
			self.name = kwargs["name"]
		if "locationid" in kwargs:
			self.locationid = kwargs["locationid"]

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)
		device = xml.SubElement(request, "device")
		device.set("euid", self.device)

		if hasattr(self, "name"):
			device.set("name", self.name)
		if hasattr(self, "locationid"):
			device.set("locationid", self.locationid)

		return request

class DeviceUnregister(Request):
	def __init__(self, gateway, device, sessionid, **kwargs):
		Request.__init__(self, ns = "devices",
			type = "unregister", sessionid = sessionid)
		self.gateway = gateway
		self.device = device

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)
		device = xml.SubElement(request, "device")
		device.set("euid", self.device)

		return request

class DeviceGetLog(Request):
	def __init__(self, gateway, device, module, sessionid, **kwargs):
		Request.__init__(self, ns = "devices",
			type = "getlog", sessionid = sessionid)
		self.gateway = gateway
		self.device = device
		self.module = module

		self.start = int(time.mktime(kwargs["start"].timetuple()))
		self.end = int(time.mktime(kwargs["end"].timetuple()))
		self.ftype = kwargs["ftype"]
		if "interval" in kwargs:
			self.interval = kwargs["interval"]
		else:
			self.interval = 5

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)
		logs = xml.SubElement(request, "logs")
		logs.set("deviceeuid", str(self.device))
		logs.set("moduleid", str(self.module))
		logs.set("from", str(self.start))
		logs.set("to", str(self.end))
		logs.set("ftype", self.ftype)
		logs.set("interval", str(self.interval))

		return request

class RoleInvite(Request):
	def __init__(self, gateway, email, sessionid, **kwargs):
		Request.__init__(self, ns = "gateusers",
			type = "invite", sessionid = sessionid)
		self.gateway = gateway
		self.email = email

		if "permission" in kwargs:
			self.permission = kwargs["permission"]
		else:
			self.permission = "user"

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)
		user = xml.SubElement(request, "user")
		user.set("email", self.email)
		user.set("permission", self.permission)

		return request

class RoleGetAll(Request):
	def __init__(self, gateway, sessionid, **kwargs):
		Request.__init__(self, ns = "gateusers",
			type = "getall", sessionid = sessionid)
		self.gateway = gateway

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)

		return request

class RoleRemove(Request):
	def __init__(self, gateway, id, sessionid, **kwargs):
		Request.__init__(self, ns = "gateusers",
			type = "remove", sessionid = sessionid)
		self.gateway = gateway
		self.id = id

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)
		user = xml.SubElement(request, "user")
		user.set("id", self.id)

		return request

class RoleUpdateAccess(Request):
	def __init__(self, gateway, id, level, sessionid, **kwargs):
		Request.__init__(self, ns = "gateusers",
			type = "updateaccess", sessionid = sessionid)
		self.gateway = gateway
		self.id = id
		self.level = level

	def xml(self):
		request = Request.xml(self)
		request.set("gateid", self.gateway)
		user = xml.SubElement(request, "user")
		user.set("id", self.id)
		user.set("permission", self.level)

		return request
