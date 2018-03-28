#include <vector>
#include <tuple>

#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "jwt/JWToken.h"
#include "model/TokenID.h"
#include "server/JWTSessionManager.h"
#include "util/Base64.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, JWTSessionManager)
BEEEON_OBJECT_CASTABLE(SessionManager)
BEEEON_OBJECT_PROPERTY("audienceList", &JWTSessionManager::setAudienceList)
BEEEON_OBJECT_PROPERTY("jwtDecoder", &JWTSessionManager::setJWTDecoder)
BEEEON_OBJECT_PROPERTY("jwtEncoder", &JWTSessionManager::setJWTEncoder)
BEEEON_OBJECT_PROPERTY("localeManager", &JWTSessionManager::setLocaleManager)
BEEEON_OBJECT_PROPERTY("issuerName", &JWTSessionManager::setIssuerName)
BEEEON_OBJECT_PROPERTY("sessionExpireTime", &JWTSessionManager::setSessionExpireTime)
BEEEON_OBJECT_END(BeeeOn, JWTSessionManager)

static const size_t UUID_SIZE = 16;

JWTSessionManager::JWTSessionManager()
{
}

void JWTSessionManager::setSessionExpireTime(const Timespan &time)
{
	m_expireTime = time;
}

void JWTSessionManager::setLocaleManager(LocaleManager::Ptr localeManager)
{
	m_localeManager = localeManager;
}

void JWTSessionManager::setJWTDecoder(const JWTDecoder::Ptr decoder)
{
	m_decoder = decoder;
}

void JWTSessionManager::setJWTEncoder(const JWTEncoder::Ptr encoder)
{
	m_encoder = encoder;
}

void JWTSessionManager::setAudienceList(const list<string> &audience)
{
	for (auto i : audience)
		m_audienceList.push_back(i);
}

void JWTSessionManager::setIssuerName(const string &issuer)
{
	m_issuerName = issuer;
}

/*
 * The function takes IDs of the user and identity, converts them to raw bytes
 * representation and returns a Base64-encoded string holding both values.
 */
static string encodeSubject(const User &user, const VerifiedIdentity &identity)
{
	vector<uint8_t> subject = user.id().toBytes();
	const vector<uint8_t> &tmp = identity.id().toBytes();
	subject.insert(subject.begin(), tmp.begin(), tmp.end());

	return Base64::encode(reinterpret_cast<char *>(subject.data()), subject.size());
}

/*
 * Decodes the Base64-encoded string holding two UUID values in the raw form
 * as bytes. Returns the UserID and VerifiedIdentityID decoded from the data.
 */
static tuple<UserID, VerifiedIdentityID> decodeSubject(const string &subject)
{
	vector<uint8_t> raw(2 * UUID_SIZE);

	char *subjectRaw = reinterpret_cast<char *>(raw.data());
	const size_t len = Base64::decode(subject, subjectRaw, 2 * UUID_SIZE);

	if (len != 2 * UUID_SIZE)
		throw InvalidArgumentException("subject data of invalid length: " + to_string(len));

	const VerifiedIdentityID &identityID = VerifiedIdentityID::fromBytes(raw);
	const UserID &userID = UserID::fromBytes(vector<uint8_t>(raw.begin() + UUID_SIZE, raw.end()));

	return make_tuple(userID, identityID);
}

const Session::Ptr JWTSessionManager::open(
		const VerifiedIdentity &identity)
{
	if (identity.id().isNull())
		throw InvalidArgumentException("missing verified identity ID");

	if (identity.user().id().isNull())
		throw InvalidArgumentException("missing verified identity user ID");

	const User &user = identity.user();

	const string &subjectEncoded = encodeSubject(user, identity);
	if (logger().debug())
		logger().debug("encoded subject: " + subjectEncoded);

	JWToken token;
	vector<string> audience;
	Timestamp now;

	token.setIssuer(m_issuerName);
	token.setAudience(m_audienceList);
	token.setSubject(subjectEncoded);
	token.setLocale(user.locale().toString());
	token.setExpirationTime(now + m_expireTime);
	token.setIssuedAt(now);
	token.setNotBefore(now);

	const TokenID &tokenID = m_encoder->encode(token);
	const SessionID &sessionID = tokenID.toString();

	Session::Ptr session = new Session(sessionID);
	session->setUserID(user.id());
	session->setExpiration(Timestamp{} + m_expireTime);
	session->setIdentityID(identity.id());
	session->setLocale(user.locale());

	if (logger().debug()) {
		logger().debug("Created session: " + session->sessionID(),
				__FILE__, __LINE__);
	}

	return session;
}

bool JWTSessionManager::lookup(const SessionID &id, Session::Ptr &session)
{
	JWToken token;

	if (logger().debug())
		logger().debug("decoding session data from: " + id, __FILE__, __LINE__);

	try {
		token = m_decoder->decode(TokenID::parse(id));
	} catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		return false;
	}

	Timestamp now;

	if (token.hasExpired(now))
		return false;
	if (!token.isAcceptable(now))
		return false;

	if (!token.isInAudience(m_issuerName)) {
		if (logger().debug())
			logger().debug("audience does not contain " + m_issuerName);

		return false;
	}

	if (token.subject().empty()) {
		if (logger().debug())
			logger().debug("token has unspecified subject");

		return false;
	}

	Timespan expTime = token.expirationTime().value().elapsed();

	VerifiedIdentityID identityID;
	UserID userID;

	tie(userID, identityID) = decodeSubject(token.subject());

	session = new Session(id);
	session->setUserID(userID);
	session->setExpiration(Timestamp{} + expTime);
	session->setIdentityID(identityID);

	// when not found default locale is set
	session->setLocale(m_localeManager->parse(token.locale()));

	return true;
}

void JWTSessionManager::close(const SessionID &id)
{
}
