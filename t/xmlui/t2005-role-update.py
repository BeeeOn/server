#! /usr/bin/env python3

import config
config.import_libs()

import time
import unittest

from xmlui import Connector, Response, Login, Logout
from xmlui import GatewayRegister, GatewayUnregister, GatewayGet
from xmlui import RoleInvite, RoleGetAll, RoleUpdateAccess

class TestRoleUpdate(unittest.TestCase):
	"""
	Login two users (Joe and John) and register Joe into the
	empty gateway. Invite John as user. Obtain both users' IDs.
	"""
	def setUp(self):
		self.conn = Connector(config.xmlui_host, config.xmlui_port, config.xmlui_ssl)

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

		time.sleep(1) # wait to prevent db constraint violation
		response = self.conn.request(RoleInvite(
			config.tmp_gateway_id,
			"john.smith@example.org",
			self.session,
			permission = "user"
		))

		self.assertTrue(response.is_ok())

		response = self.conn.request(RoleGetAll(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_data())
		self.assertEqual(4, len(response.root))
		self.assertEqual("joe.doe@example.org", response.root[0].get("email"))
		self.assertEqual("Joe", response.root[0].get("name"))
		self.assertEqual("Doe", response.root[0].get("surname"))
		self.assertEqual("admin", response.root[0].get("level"))
		self.assertEqual("owner", response.root[0].get("permission"))
		self.assertEqual("joe.doe@example.org", response.root[1].get("email"))
		self.assertEqual("Joe", response.root[1].get("name"))
		self.assertEqual("Doe", response.root[1].get("surname"))
		self.assertEqual("admin", response.root[1].get("level"))
		self.assertEqual("owner", response.root[1].get("permission"))

		self.assertEqual(response.root[0].get("id"), response.root[1].get("id"))

		self.assertEqual("john.smith@example.org", response.root[2].get("email"))
		self.assertEqual("John", response.root[2].get("name"))
		self.assertEqual("Smith", response.root[2].get("surname"))
		self.assertEqual("user", response.root[2].get("level"))
		self.assertEqual("user", response.root[2].get("permission"))
		self.assertEqual("john.smith@example.org", response.root[3].get("email"))
		self.assertEqual("John", response.root[3].get("name"))
		self.assertEqual("Smith", response.root[3].get("surname"))
		self.assertEqual("user", response.root[3].get("level"))
		self.assertEqual("user", response.root[3].get("permission"))

		self.assertEqual(response.root[2].get("id"), response.root[3].get("id"))

		self.joe = response.root[0].get("id")
		self.john = response.root[2].get("id")

	"""
	Unregister the empty gateway and logout both users.
	"""
	def tearDown(self):
		response = self.conn.request(GatewayUnregister(
			config.tmp_gateway_id,
			self.session2
		))

		response = self.conn.request(GatewayUnregister(
			config.tmp_gateway_id,
			self.session
		))

		self.assertTrue(response.is_ok())
		response = self.conn.request(Logout(self.session2))
		self.assertTrue(response.is_ok())

		response = self.conn.request(Logout(self.session))
		self.assertTrue(response.is_ok())

	"""
	Cannot change own access level to guest nor to user.
	"""
	def test1_cannot_downgrade_myself(self):
		response = self.conn.request(RoleUpdateAccess(
			config.tmp_gateway_id,
			self.joe,
			"guest",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("1001", response.error_code())

		self.conn.request(RoleUpdateAccess(
			config.tmp_gateway_id,
			self.joe,
			"user",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("1001", response.error_code())

	"""
	Cannot reapply own's admin access level.
	This is a non-sense operation, just make sure how does it behave.
	"""
	def test2_cannot_update_myself_atall(self):
		response = self.conn.request(RoleUpdateAccess(
			config.tmp_gateway_id,
			self.joe,
			"admin",
			self.session
		))

		self.assertTrue(response.is_error())
		self.assertEqual("1001", response.error_code())

	"""
	John cannot change own's access level to admin.
	"""
	def test3_cannot_upgrade_myself(self):
		response = self.conn.request(RoleUpdateAccess(
			config.tmp_gateway_id,
			self.john,
			"admin",
			self.session2
		))

		self.assertTrue(response.is_error())
		self.assertEqual("1001", response.error_code())

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
