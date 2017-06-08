#! /usr/bin/env python3

import json
from urllib.parse import urlparse, parse_qs

# import Google OAuth Client library
try:
	from oauth2client.client import OAuth2WebServerFlow

	def manual():
		return True
except ImportError as e:
	def manual():
		return False

# import Selenium for automatic web login
try:
	from selenium import webdriver
	from selenium.common.exceptions import WebDriverException
	from selenium.webdriver.support import expected_conditions
	from selenium.webdriver.support.ui import WebDriverWait
	from selenium.webdriver.common.by import By

	def automatic():
		"""
		Fully automatic interaction with web login approaches is available.
		"""
		return True

	def webdriver_create(name):
		if name == "firefox":
			return webdriver.Firefox()
		elif name == "chrome":
			return webdriver.Chrome()
		elif name == "ie":
			return webdriver.Ie()
		else:
			return webdriver.PhantomJS(
				service_log_path="weblogin_phantomjs.log",
				service_args=["--webdriver-loglevel=DEBUG"]
			)

	def webdriver_is_phantomjs(driver):
		return isinstance(driver, webdriver.PhantomJS)

except ImportError as e:
	def automatic():
		"""
		Selenium is unavailable, cannot automate web login interaction.
		"""
		return False

class Result:
	def __init__(self, kind, value):
		self._kind = kind
		self._value = value

	def kind(self):
		return self._kind

	def value(self):
		return self._value

	def __repr__(self):
		return "%s %s" % (self.kind(), self.value())

class CodeResult(Result):
	"""
	Result contains the authentication code.
	"""
	def __init__(self, code):
		Result.__init__(self, "code", code)

class ManualResult(Result):
	"""
	Result contains URI to obtain the authentication code.
	This probably means that selenium is unavailable.
	"""
	def __init__(self, login_uri):
		Result.__init__(self, "manual", login_uri)

class AbstractLogin:
	def _login_uri(self, scope, redirect = "http://localhost"):
		"""
		Provide login URI only.
		"""
		flow = OAuth2WebServerFlow(
				self._id,
				scope = scope,
				redirect_uri = redirect,
				auth_uri = self._auth_uri
		)
		return flow.step1_get_authorize_url()

	def _config_driver(self, **kwargs):
		"""
		Configures driver so that it can be used in other autorisation functions.

		* 'webdriver' - selenium webdriver, default: 'phantomjs'
		* 'window' - window size, default: (1024, 768)
		"""
		if "webdriver" in kwargs:
			driver = webdriver_create(kwargs["webdriver"])
		else:
			driver = webdriver_create("phantomjs")

		if "window" in kwargs:
			w = kwargs["window"]
		else:
			w = [1024, 768]

		driver.set_window_size(w[0], w[1])
		return driver

	def _wait_clickable(self, driver, id, timeout = 30):
		"""
		Wait until the element identified by the given ID is clickable.
		"""
		cond = expected_conditions.element_to_be_clickable((By.ID, id))
		WebDriverWait(driver, timeout).until(cond)

	def _wait_clickable_name(self, driver, name, timeout = 30):
		"""
		Wait until the element identified by the given name is clickable.
		"""
		cond = expected_conditions.element_to_be_clickable((By.NAME, name))
		WebDriverWait(driver, timeout).until(cond)

	def _wait_while_present_name(self, driver, name, timeout = 30):
		"""
		Wait until the element identified by the given ID is clickable.
		"""
		WebDriverWait(driver, timeout).until_not(lambda x: x.find_element_by_name(name).is_displayed())

	def _extract_code(self, driver):
		"""
		Extract the authentication code. This is a tricky operation when using
		PhantomJS as it does not process 301 redirects properly. Workaround:
		list the PhantomJS log and find message/log/entries/request/url entry
		that contains the code encoded in the URI. It is assumed that the URI
		is in the first log entry.
		"""
		if webdriver_is_phantomjs(driver):
			logs = driver.get_log("har")
			for e in logs:
				data = json.loads(e["message"])
				uri = data["log"]["entries"][0]["request"]["url"]
				driver.save_screenshot("extr.png");
				return CodeResult(parse_qs(urlparse(uri).query)["code"][0])

		# implemented for PhantomJS only
		raise Exception("Not implemented or failed")

GOOGLE_AUTH_URI = "https://accounts.google.com/o/oauth2/auth"
GOOGLE_BASIC_SCOPES = [
	"https://www.googleapis.com/auth/userinfo.profile",
	"https://www.googleapis.com/auth/userinfo.email"
]

class GoogleLogin(AbstractLogin):
	"""
	Login via Google OAuth2. If possible, selenium is used to
	perform the login automatically without any manual intervention.
	"""
	def __init__(self, id, auth_uri = GOOGLE_AUTH_URI):
		self._id = id
		self._auth_uri = auth_uri

	def weblogin(self, user, password, scope = GOOGLE_BASIC_SCOPES, **kwargs):
		"""
		If possible, logins into the Google OAuth2 service via the provided credentials
		by using selenium. Otherwise, only login URI is returned.
		"""
		if automatic():
			return self._do_weblogin(user, password, scope, **kwargs)
		elif manual():
			return ManualResult(self._login_uri(scope))
		else:
			raise Exception("missing Google OAuth2 Client libarary and Selenium")

	def _do_weblogin(self, user, password, scope = GOOGLE_BASIC_SCOPES, **kwargs):
		"""
		Perform automatic login via Google OAuth2 website. It is possible
		to override parameters:
		"""
		driver = self._config_driver(**kwargs)
		driver.get(self._login_uri(scope))

		try:
			self._input_email(driver, user)
			self._input_password(driver, password)
			self._allow_permissions(driver)
			return self._extract_code(driver)
		except WebDriverException as e:
			driver.save_screenshot("google_login_failed.png")
			raise e

	def _input_email(self, driver, user):
		"""
		Type the user e-mail into web-form when the button "next" becomes clickable.
		"""
		self._wait_clickable(driver, "next")
		email = driver.find_element_by_id("Email")
		email.send_keys(user)
		next = driver.find_element_by_id("next")
		next.click()

	def _input_password(self, driver, passwd):
		"""
		Type the user password into web-form when the button "signIn" becomes clickable.
		"""
		self._wait_clickable(driver, "signIn")
		password = driver.find_element_by_id("Passwd")
		password.send_keys(passwd)
		signIn = driver.find_element_by_id("signIn")
		signIn.click()

	def _allow_permissions(self, driver):
		"""
		Finally, approve the asked permissions for the website.
		"""
		self._wait_clickable(driver, "submit_approve_access")
		allow = driver.find_element_by_id("submit_approve_access")
		allow.click()

if __name__ == "__main__":
	import sys
	import getpass

	if len(sys.argv) < 4:
		print("oauth2.py <provider> <id> <user> [<password>]")
		print("  providers: google")
		print("  if no password it is asked on prompt")
		sys.exit(1)

	if sys.argv[1] == "google":
		login = GoogleLogin(sys.argv[2])
		if len(sys.argv) >= 5:
			result = login.weblogin(sys.argv[3], sys.argv[4])
		else:
			result = login.weblogin(sys.argv[3], getpass.getpass())

		print("%s" % (result))
	else:
		raise Exception("no such provider: %s" % (provider))
