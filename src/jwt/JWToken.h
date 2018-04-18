#pragma once

#include <set>

#include <Poco/Nullable.h>
#include <Poco/Timestamp.h>
#include <Poco/URI.h>

namespace BeeeOn {

/**
 * @brief The class represents a JSON Web Token payload according to the JWT
 * specification (extended by locale member). The header and signature
 * must be provided by some other component at the time of distribution
 * to a client.
 *
 * @see https://tools.ietf.org/html/rfc7519
 */
class JWToken {
public:
	JWToken();
	~JWToken();

	/**
	 * Issuer claim represents issuer of the token. For example
	 * BeeeOn server.
	 */
	void setIssuer(const std::string &issuer);
	std::string issuer() const;

	/**
	 * Subject claim represents user that is obtained by token.
	 */
	void setSubject(const std::string &subject);
	std::string subject() const;

	/**
	 * Audience claim identifies the recipients that the JWT is
	 * intended for. Audience is array of all recipients that
	 * are intended to process token.
	 */
	void setAudience(const std::set<std::string> &aud);
	std::set<std::string> audience() const;

	/**
	 * Expiration claim identifies the expiration time on (or
	 * after) which the JWT MUST NOT be accepted for processing.
	 */
	void setExpirationTime(const Poco::Timestamp &times);
	Poco::Nullable<Poco::Timestamp> expirationTime() const;

	/**
	 * Issued at claim identifies the time at which the JWT was
	 * issued.
	 */
	void setIssuedAt(const Poco::Timestamp &time);
	Poco::Nullable<Poco::Timestamp> issuedAt() const;

	/**
	 * Not before claim identifies the time before which the JWT
	 * token cannot be accepted to be processed.
	 */
	void setNotBefore(const Poco::Timestamp &time);
	Poco::Nullable<Poco::Timestamp> notBefore() const;

	/**
	 * Claim that contains the first name of a person.
	 *
	 * @see http://ldapwiki.com/wiki/JSON%20Web%20Token%20Claims
	 * @see http://ldapwiki.com/wiki/Given_name
	 */
	void setGivenName(const std::string &name);
	Poco::Nullable<std::string> givenName() const;

	/**
	 * Claim that contains the last name of a person.
	 *
	 * @see http://ldapwiki.com/wiki/JSON%20Web%20Token%20Claims
	 * @see http://ldapwiki.com/wiki/Family_name
	 */
	void setFamilyName(const std::string &name);
	Poco::Nullable<std::string> familyName() const;

	/**
	 * Claim that contains URI pointing to the users' picture.
	 *
	 * @see http://ldapwiki.com/wiki/JSON%20Web%20Token%20Claims
	 * @see http://ldapwiki.com/wiki/Picture
	 */
	void setPicture(const Poco::URI &uri);
	Poco::Nullable<Poco::URI> picture() const;

	/**
	 * Claim that contains e-mail.
	 *
	 * @see http://ldapwiki.com/wiki/JSON%20Web%20Token%20Claims
	 * @see http://ldapwiki.com/wiki/Email
	 */
	void setEmail(const std::string &email);
	Poco::Nullable<std::string> email() const;

	/**
	 * Locale claim identifies the locale of the subject.
	 *
	 * @see http://ldapwiki.com/wiki/JSON%20Web%20Token%20Claims
	 * @see http://ldapwiki.com/wiki/Locale
	 */
	void setLocale(const std::string &locale);
	std::string locale() const;

	/**
	 * Checks whether tokens expiration time does not
	 * exceed time defined by at.
	 */
	bool hasExpired(const Poco::Timestamp &at) const;

	/**
	 * Checks whether time defined in not before claim does not
	 * exceed time defined as startTime.
	 */
	bool isAcceptable(const Poco::Timestamp &startTime) const;

	/**
	 * Checks whether audience of the token does contain just
	 * recipients defined in audience list passed as audience.
	 */
	bool isInAudience(const std::string &recipient) const;

private:
	std::string m_issuer;
	std::string m_locale;
	std::string m_subject;
	std::set<std::string> m_audience;
	Poco::Nullable<Poco::Timestamp> m_expiration;
	Poco::Nullable<Poco::Timestamp> m_issuedAt;
	Poco::Nullable<Poco::Timestamp> m_notbefore;
	Poco::Nullable<std::string> m_givenName;
	Poco::Nullable<std::string> m_familyName;
	Poco::Nullable<Poco::URI> m_picture;
	Poco::Nullable<std::string> m_email;
};

}
