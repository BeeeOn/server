import os
import json

def skip_login():
	return not "GOOGLE_AUTH_CODE" in os.environ

def login_auth_code():
	return os.environ["GOOGLE_AUTH_CODE"]
