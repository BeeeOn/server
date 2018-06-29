#pragma once

#include <string>

#include <Poco/SharedPtr.h>

#include "jwt/JWTDecoder.h"
#include "oauth2/OAuth2UserFetcher.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief OpenIDUserDecoder decodes ID tokens and provide the OpenID
 * user information the token contains.
 */
class OpenIDUserDecoder : public OAuth2UserFetcher, Loggable {
public:
	typedef Poco::SharedPtr<OpenIDUserDecoder> Ptr;

	OpenIDUserDecoder();

	void setDecoder(JWTDecoder::Ptr decoder);
	void setClientId(const std::string &clientId);

	std::string clientId() const override;

	void fetch(
		const OAuth2CodeExchanger::Tokens &tokens,
		AuthResult &result,
		const Poco::Timestamp &now = {}) override;

protected:
	JWTDecoder::Ptr m_decoder;
	std::string m_clientId;
};

}

