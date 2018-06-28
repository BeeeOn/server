import os

def import_libs():
	from os import sys, path
	sys.path.append(path.dirname(path.dirname(path.abspath(__file__))))

import_libs()

gws_host = os.getenv("GWS_SERVER_HOST", "localhost")
gws_port = int(os.getenv("GWS_SERVER_PORT", 8850))
gws_ws_uri = "ws://%s:%u" %(gws_host, gws_port)
gws_zmq_endpoint = os.getenv("GWS_SERVER_ZMQ_ENDPOINT", "tcp://127.0.0.1:8860")

gateway_id = "1284174504043136"
