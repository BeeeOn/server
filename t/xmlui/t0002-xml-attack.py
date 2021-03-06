#! /usr/bin/env python3

import config
config.import_libs()

import unittest
import socket
import xml.etree.ElementTree as xml

from xmlui import Response

class TestXmlAttack(unittest.TestCase):
	"""
	Test server refuses XML starting with <!DOCTYPE...
	"""
	def test1_refuse_doctype_xml(self):
		s = socket.socket()

		if config.xmlui_ssl:
			import ssl
			s = ssl.wrap_socket(s)

		s.connect((config.xmlui_host, config.xmlui_port))
		s.sendall(bytes("<!DOCTYPE s [ <!ENTITY n \"LLL\"> ]>" +
				"<request version=\"1.0.0\" ns=\"&n;\" />",
				"utf-8"))

		raw = s.recv(1024)
		s.close()

		self.assertEqual(0, len(raw))

	def test2_too_long_message(self):
		s = socket.socket()

		if config.xmlui_ssl:
			import ssl
			s = ssl.wrap_socket(s)

		s.connect((config.xmlui_host, config.xmlui_port))
		s.send(bytes("<message>", "utf-8"))

		try:
			while True:
				s.send(bytes("<item>" + ("x" * 1010) + "</item>", "utf-8"))
		except Exception as e:
			self.assertTrue(True, str(e))

if __name__ == '__main__':
	import sys
	import taprunner
	unittest.main(testRunner=taprunner.TAPTestRunner(stream = sys.stdout))
