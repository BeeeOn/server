import os

def import_libs():
	from os import sys, path
	sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))

import_libs()

gws_host = os.getenv("GWS_SERVER_HOST", "localhost")
gws_port = int(os.getenv("GWS_SERVER_PORT", 8850))
gws_ws_uri = "ws://%s:%u" %(gws_host, gws_port)


gateway_id = "1284174504043136"
