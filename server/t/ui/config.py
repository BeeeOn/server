import json

def import_libs():
	from os import sys, path
	sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))

ui_host = "localhost"
ui_port = 8000
ui_session_per_user = 10

PERMIT_LOGIN = json.dumps(
	{"provider": "permit", "authCode": "joe.doe@example.org"})
