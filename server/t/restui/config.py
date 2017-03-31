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

PERMIT_LOGIN = json.dumps(
	{"provider": "permit", "code": "joe.doe@example.org"})
