import json
import os

def import_libs():
	from os import sys, path
	sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))

ui_host = "localhost"
ui_port = int(os.getenv("RESTUI_SERVER_PORT", 8000))
ui_session_per_user = 10

gateway_id = "1284174504043136"
tmp_gateway_id = "1780053541714013"

fcm_token = (
	"cxuhiaqxWxk:APA91bH-H8Gc690MjdlpMDX3Xc1BbhfuokwlZkInEu"
	"CsCvdwxUpTWGG0V8O_dD_Nx6qE42bjrdN4VONjPISnesoAHOXae3Wy"
	"rccyjcoKJ9OM5QUbUdN1918FyNVWITiqERaQlj5bKMAk")

PERMIT_LOGIN = json.dumps(
	{"provider": "permit", "code": "joe.doe@example.org"})
