#pragma once

#include <list>
#include <vector>

#include <Poco/Logger.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "jwt/JWTDecoder.h"
#include "jwt/JWTEncoder.h"
#include "l10n/LocaleManager.h"
#include "model/VerifiedIdentity.h"
#include "server/Session.h"
#include "server/SessionManager.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief JWTSessionManager provides session managing via
 * JSON Web Tokens.
 *
 * To validate user by token JWTSessionManager expects
 * values of this fields of claims of JWToken to be
 * valid:
 *
 *  * issuer
 *   - Name of known issuer that encoded token.
 *   - JWTSessionManager issues tokens with its
 *     own issuer Name.
 *
 *  * subject
 *   - Identification of user authentifying with token.
 *
 *  * audience
 *   - List of all recipients that are able to verify
 *     validity of token.
 *   - JWTSessionManager thus must find its issuer name
 *     in audience list.
 *
 *  * expiration
 *   - NumericDate format of time of expiration of the token.
 *   - After time of expiration token is considered not valid.
 *
 *  * issuedAt
 *   - NumericDate format of time when token was issued.
 *
 *  * notBefore
 *   - NumericDate format of time before which token must be
 *     considered not valid.
 *
 *  * locale
 *   - language_COUNTRY format of the locale of the user.
 */
class JWTSessionManager : public SessionManager, Loggable {
public:
	JWTSessionManager();

	void setSessionExpireTime(const Poco::Timespan &time);
	void setJWTDecoder(const JWTDecoder::Ptr decoder);
	void setJWTEncoder(const JWTEncoder::Ptr encoder);
	void setLocaleManager(LocaleManager::Ptr localeManager);
	void setAudienceList(const std::list<std::string> &audience);
	void setIssuerName(const std::string &issuers);

	/**
	 * Creates JWT and fills audience and issuer by setting specification.
	 * Subject of JWToken is filled with userID and verifiedIdentityID.
	 */
	const ExpirableSession::Ptr open(const VerifiedIdentity &identity) override;

	/**
	 * Decodes and verify received token. If list of audience does not include
	 * issuer name of JWTokenSessionManager token is refused. Otherwise token
	 * is attempted to be decoded.
	 */
	bool lookup(const SessionID &id, ExpirableSession::Ptr &info) override;

	/**
	 * Does nothing in this implementation.
	 */
	void close(const SessionID &id) override;

private:
	JWTDecoder::Ptr m_decoder;
	JWTEncoder::Ptr m_encoder;
	LocaleManager::Ptr m_localeManager;
	Poco::Timespan m_expireTime;
	std::string m_issuerName;
	std::vector<std::string> m_audienceList;
};

}
