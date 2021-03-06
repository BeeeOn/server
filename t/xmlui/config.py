import os

def import_libs():
	from os import sys, path
	sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))

import_libs()
import xmlui

xmlui_host = os.getenv("XMLUI_SERVER_HOST", "localhost")
xmlui_port = int(os.getenv("XMLUI_SERVER_PORT", 8001))

if os.getenv("XMLUI_SERVER_SSL", 0) == 0:
	xmlui_ssl = False
else:
	xmlui_ssl = True

xmlui_session_per_user = 10

gateway_id = "1284174504043136"
tmp_gateway_id = "1780053541714013"

fcm_token = (
	"cxuhiaqxWxk:APA91bH-H8Gc690MjdlpMDX3Xc1BbhfuokwlZkIn"
	"EuCsCvdwxUpTWGG0V8O_dD_Nx6qE42bjrdN4VONjPISnesoAHOXa"
	"e3WyrccyjcoKJ9OM5QUbUdN1918FyNVWITiqERaQlj5bKMAk")

PERMIT_LOGIN = xmlui.Login("permit", "joe.doe@example.org")
PERMIT_LOGIN2 = xmlui.Login("permit", "john.smith@example.org")
