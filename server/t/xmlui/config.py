import os

def import_libs():
	from os import sys, path
	sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))

import_libs()
import xmlui

xmlui_host = "localhost"
xmlui_port = int(os.getenv("XMLUI_SERVER_PORT", 8001))
xmlui_session_per_user = 10

gateway_id = "1284174504043136"
tmp_gateway_id = "1780053541714013"

PERMIT_LOGIN = xmlui.Login("permit", "joe.doe@example.org")
PERMIT_LOGIN2 = xmlui.Login("permit", "john.smith@example.org")
