#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "oauth2/GenericCodeExchanger.h"
#include "util/JsonUtil.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GenericCodeExchanger)
BEEEON_OBJECT_CASTABLE(OAuth2CodeExchanger)
BEEEON_OBJECT_PROPERTY("redirectUri", &GenericCodeExchanger::setRedirectUri)
BEEEON_OBJECT_PROPERTY("codeKey", &GenericCodeExchanger::setCodeKey)
BEEEON_OBJECT_PROPERTY("errorKey", &GenericCodeExchanger::setErrorKey)
BEEEON_OBJECT_PROPERTY("errorMessageKey", &GenericCodeExchanger::setErrorMessageKey)
BEEEON_OBJECT_PROPERTY("requestData", &GenericCodeExchanger::setRequestData)
BEEEON_OBJECT_PROPERTY("exchangeUrl", &GenericCodeExchanger::setExchangeUrl)
BEEEON_OBJECT_PROPERTY("sslConfig", &GenericCodeExchanger::setSSLConfig)
BEEEON_OBJECT_END(BeeeOn, GenericCodeExchanger)


using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Net;
using namespace BeeeOn;

GenericCodeExchanger::GenericCodeExchanger():
	m_redirectUri("http://localhost"),
	m_codeKey("code"),
	m_errorKey("error"),
	m_errorMessageKey("message"),
	m_formEncoding(HTMLForm::ENCODING_URL),
	m_exchanger(HTTPRequest::HTTP_POST, "http://localhost/oauth2")
{
}

void GenericCodeExchanger::setExchangeUrl(const string &url)
{
	m_exchanger.setURL(URI(url));
}

void GenericCodeExchanger::setSSLConfig(SSLClient::Ptr config)
{
	m_exchanger.setSSLConfig(config);
}

void GenericCodeExchanger::setRedirectUri(const string &uri)
{
	m_redirectUri = uri;
}

string GenericCodeExchanger::redirectUri() const
{
	return m_redirectUri;
}

void GenericCodeExchanger::setCodeKey(const string &key)
{
	m_codeKey = key;
}

string GenericCodeExchanger::codeKey() const
{
	return m_codeKey;
}

void GenericCodeExchanger::setErrorKey(const string &key)
{
	m_errorKey = key;
}

string GenericCodeExchanger::errorKey() const
{
	return m_errorKey;
}

void GenericCodeExchanger::setErrorMessageKey(const string &key)
{
	m_errorMessageKey = key;
}

string GenericCodeExchanger::errorMessageKey() const
{
	return m_errorMessageKey;
}

void GenericCodeExchanger::setFormEncoding(const string &encoding)
{
	m_formEncoding = encoding;
}

string GenericCodeExchanger::formEncoding() const
{
	return m_formEncoding;
}

void GenericCodeExchanger::setRequestData(const map<string, string> &data)
{
	m_requestData = data;
}

OAuth2CodeExchanger::Tokens GenericCodeExchanger::exchange(
		const AuthCodeCredentials &cred)
{
	HTMLForm form;
	form.setEncoding(m_formEncoding);

	for (const auto &pair : m_requestData)
		form.set(pair.first, pair.second);

	form.set(m_codeKey, cred.authCode());

	Object::Ptr body = JsonUtil::parse(m_exchanger.requestBody(form));

	if (!m_errorKey.empty() && body->has(m_errorKey)) {
		const string message = body->getValue<string>(m_errorMessageKey);
		throw NotAuthenticatedException(message);
	}

	return OAuth2CodeExchanger::extractTokens(body);
}
