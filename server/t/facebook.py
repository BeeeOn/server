import os
import json
from oauth2 import FacebookLogin

def skip_login():
	if "FACEBOOK_AUTH_CODE" in os.environ:
		return False

	import oauth2
	if oauth2.automatic():
		if not "FACEBOOK_USER" in os.environ:
			return True
		if not "FACEBOOK_PASSWORD" in os.environ:
			return True
		if not "FACEBOOK_CLIENT_ID" in os.environ:
			return True

		return False

	return True


def login_auth_code():
	if "FACEBOOK_AUTH_CODE" in os.environ:
		return os.environ["FACEBOOK_AUTH_CODE"]
	else:
		login = FacebookLogin(os.environ["FACEBOOK_CLIENT_ID"])
		result = login.weblogin(os.environ["FACEBOOK_USER"],
				os.environ["FACEBOOK_PASSWORD"])
		if result.kind() == "code":
			return result.value()
		else:
			raise Exception("Facebook login result: %s" % (result))
