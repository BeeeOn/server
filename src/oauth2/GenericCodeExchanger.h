#pragma once

#include <map>
#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "oauth2/OAuth2CodeExchanger.h"
#include "util/HTTPClient.h"

namespace BeeeOn {

/**
 * @brief GenericCodeExchanger provides a configurable generic
 * method to perform the authorization code flow to obtain OAuth2
 * tokens.
 */
class GenericCodeExchanger : public OAuth2CodeExchanger {
public:
	GenericCodeExchanger();

	void setExchangeUrl(const std::string &url);
	void setSSLConfig(SSLClient::Ptr config);

	void setRedirectUri(const std::string &uri);
	std::string redirectUri() const override;

	/**
	 * @brief Set key of HTTP param that would hold the
	 * authorization code sending to the exchange server.
	 */
	void setCodeKey(const std::string &key);
	std::string codeKey() const;

	/**
	 * @brief Set key of JSON response that if present signalizes
	 * an error condition.
	 */
	void setErrorKey(const std::string &key);
	std::string errorKey() const;

	/**
	 * @brief Set key of JSON response that would contain an error
	 * message in case of an error (the errorKey was found).
	 */
	void setErrorMessageKey(const std::string &key);
	std::string errorMessageKey() const;

	/**
	 * @brief Set encoding of the request (like HTML form). Default
	 * value is: "application/x-www-form-urlencoded".
	 */
	void setFormEncoding(const std::string &encoding);
	std::string formEncoding() const;

	/**
	 * @brief Set HTTP params to be send when requesting an exchange.
	 * It would contain params like client ID, client secrent and others.
	 */
	void setRequestData(const std::map<std::string, std::string> &data);

	/**
	 * @brief Exchange the given authorization code for token.
	 */
	Tokens exchange(const AuthCodeCredentials &cred) override;

private:
	std::string m_redirectUri;
	std::string m_codeKey;
	std::string m_errorKey;
	std::string m_errorMessageKey;
	std::string m_formEncoding;
	std::map<std::string, std::string> m_requestData;
	HTTPClient m_exchanger;
};

}
