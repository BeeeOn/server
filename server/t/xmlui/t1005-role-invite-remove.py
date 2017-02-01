#! /usr/bin/env python3

import config
config.import_libs()

import time
import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GatewayRegister, GatewayUnregister, GatewayGet
from xmlui import RoleInvite, RoleGetAll, RoleRemove

class TestRoleInviteRemove(unittest.TestCase):
	"""
	Login two users (Joe and John) and register Joe into the
	empty gateway.
	"""
	def setUp(self):
		self.conn = Connector(config.xmlui_host, config.xmlui_port)

		response = self.conn.request(config.PERMIT_LOGIN)
		self.assertTrue(response.is_data())
		self.session = response.sessionid()

		response = self.conn.request(config.PERMIT_LOGIN2)
		self.assertTrue(response.is_data())
		self.session2 = response.sessionid()

		response = self.conn.request(GatewayRegister(
			config.tmp_gateway_id,
			self.session,
			name = "Test Gateway"
		))

		self.assertTrue(response.is_ok())

	"""
	Unregister the empty gateway and logout both users.
	"""
	def tearDown(self):
		response = self.conn.request(GatewayUnregister(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_ok())
		response = self.conn.request(Logout(self.session2))
		self.assertTrue(response.is_ok())

		response = self.conn.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	def check_john_cannot_access(self, gateway):
		response = self.conn.request(GatewayGet(
			gateway,
			self.session2
		))

		self.assertTrue(response.is_error())
		self.assertEqual("1001", response.error_code())

	def check_john_can_access(self, gateway, role):
		response = self.conn.request(GatewayGet(
			gateway,
			self.session2
		))

		self.assertTrue(response.is_data())
		self.assertEqual("Test Gateway",
				response.root[0].get("name"))
		self.assertEqual(gateway, response.root[0].get("id"))
		self.assertEqual("2", response.root[0].get("users"))
		self.assertEqual(role, response.root[0].get("permission"))
		self.assertEqual("Joe Doe", response.root[0].get("owner"))

	"""
	Joe invites John. John gets the gateway and assures its permission.
	"""
	def test1_joe_invites_john(self):
		self.check_john_cannot_access(config.tmp_gateway_id)

		time.sleep(1) # wait to prevent db constraint violation
		response = self.conn.request(RoleInvite(
			config.tmp_gateway_id,
			"john.smith@example.org",
			self.session,
			permission = "admin"
		))

		self.assertTrue(response.is_ok())

		self.check_john_can_access(config.tmp_gateway_id, "admin")

		response = self.conn.request(RoleGetAll(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_data())
		self.assertEqual(4, len(response.root))
		self.assertEqual("joe.doe@example.org", response.root[0].get("email"))
		self.assertEqual("joe.doe@example.org", response.root[1].get("email"))
		self.assertEqual("john.smith@example.org", response.root[2].get("email"))
		self.assertEqual("john.smith@example.org", response.root[3].get("email"))

		self.assertEqual(response.root[0].get("id"), response.root[1].get("id"))
		self.assertEqual(response.root[2].get("id"), response.root[3].get("id"))

		johnRole = response.root[2].get("id")

		response = self.conn.request(RoleRemove(
			config.tmp_gateway_id,
			johnRole,
			self.session
		))

		self.assertTrue(response.is_ok());

	"""
	Joe cannot remove himself from the gateway. Only unregister is possible
	in that case.
	"""
	def test2_cannot_remove_myself(self):
		response = self.conn.request(RoleGetAll(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_data())
		self.assertEqual(2, len(response.root))
		self.assertEqual("joe.doe@example.org", response.root[0].get("email"))

		response = self.conn.request(RoleRemove(
			config.tmp_gateway_id,
			response.root[0].get("id"),
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

	"""
	Joe cannot invite himself because he has already a role.
	"""
	def test3_cannot_invite_myself(self):
		response = self.conn.request(RoleGetAll(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_data())
		self.assertEqual(2, len(response.root))
		self.assertEqual("joe.doe@example.org", response.root[0].get("email"))

		time.sleep(1) # wait to prevent db constraint violation
		response = self.conn.request(RoleInvite(
			config.tmp_gateway_id,
			"joe.doe@example.org",
			self.session,
			permission = "admin"
		))

		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

	"""
	John as guest cannot invite other users, especially not as
	new admins.
	"""
	def test4_cannot_invite_when_not_admin(self):
		self.check_john_cannot_access(config.tmp_gateway_id)

		time.sleep(1) # wait to prevent db constraint violation
		response = self.conn.request(RoleInvite(
			config.tmp_gateway_id,
			"john.smith@example.org",
			self.session,
			permission = "guest"
		))

		self.assertTrue(response.is_ok())

		self.check_john_can_access(config.tmp_gateway_id, "guest")

		response = self.conn.request(RoleInvite(
			config.tmp_gateway_id,
			"unknown@example.org",
			self.session,
			permission = "admin"
		))

		self.assertTrue(response.is_error())
		self.assertEqual("998", response.error_code())

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
