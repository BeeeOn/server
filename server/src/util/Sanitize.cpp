#include <Poco/RegularExpression.h>
#include <Poco/TextEncoding.h>
#include <Poco/TextConverter.h>
#include <Poco/UTF8Encoding.h>
#include <Poco/UTF8String.h>

#include "util/Sanitize.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

string Sanitize::encoding(const string &bytes,
		const unsigned long sizeLimit,
		const string &inputEncoding)
{
	const TextEncoding &encoding = TextEncoding::byName(inputEncoding);
	const UTF8Encoding utf8;

	if (sizeLimit > 0 && bytes.size() > sizeLimit)
		throw InvalidArgumentException("input is too long");

	TextConverter text(encoding, utf8);
	string result;
	text.convert(bytes, result);

	return result;
}

// Based on:
// https://www.w3.org/TR/html/sec-forms.html#valid-e-mail-address
// http://www.regular-expressions.info/email.html

#define EMAIL_LOCAL_PART_MAX      64
#define EMAIL_DOMAIN_PART_MAX     255
#define EMAIL_LOCAL_PART_REGEX "[a-z0-9\\.!#$%&'*+\\\\/=?^_`{|}~-]+"
#define EMAIL_DOMAIN_PART_REGEX "[a-z0-9\\.-]+\\.[a-z0-9]{2,63}"

static RegularExpression emailLocalPartRegex(EMAIL_LOCAL_PART_REGEX,
	RegularExpression::RE_DOLLAR_ENDONLY  | // $ matchs only End-of-String
	RegularExpression::RE_NO_AUTO_CAPTURE | // do not numbered captures
	RegularExpression::RE_UTF8,
	true
);

static RegularExpression emailDomainPartRegex(EMAIL_DOMAIN_PART_REGEX,
	RegularExpression::RE_DOLLAR_ENDONLY  | // $ matchs only End-of-String
	RegularExpression::RE_NO_AUTO_CAPTURE | // do not numbered captures
	RegularExpression::RE_UTF8,
	true
);

static bool match(const RegularExpression &exp, const string &s, size_t offset = 0)
{
	// implies RE_ANCHORED and RE_NOTEMPTY
	return exp.match(s, offset);
}

string Sanitize::email(const string &bytes,
		const unsigned long sizeLimit,
		const string &inputEncoding)
{
	string email(Sanitize::encoding(bytes, sizeLimit, inputEncoding));
	UTF8::toLowerInPlace(email);

	size_t at = email.find_last_of('@');
	if (at == string::npos)
		throw InvalidAccessException("e-mail is invalid: missing '@'");

	if (at > EMAIL_LOCAL_PART_MAX)
		throw InvalidAccessException("e-mail is invalid: too long local-part");

	if (email.size() - (at + 1) > EMAIL_DOMAIN_PART_MAX)
		throw InvalidAccessException("e-mail is invalid: too long domain-part");

	if (email.find("..") != string::npos)
		throw InvalidAccessException("e-mail is invalid: unexpected characters");

	if (!match(emailLocalPartRegex, email.substr(0, at)))
		throw InvalidAccessException("e-mail is invalid: bad local-part");

	if (!match(emailDomainPartRegex, email, at + 1))
		throw InvalidAccessException("e-mail is invalid: bad domain-part");

	return email;
}
