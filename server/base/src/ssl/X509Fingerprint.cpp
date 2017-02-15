#include <sstream>

#include <Poco/Exception.h>
#include <Poco/Token.h>
#include <Poco/StreamTokenizer.h>
#include <Poco/Crypto/DigestEngine.h>
#include <Poco/Crypto/X509Certificate.h>

#include "util/Base64.h"
#include "ssl/X509Fingerprint.h"

using namespace std;
using namespace Poco;
using namespace Poco::Crypto;
using namespace BeeeOn;

namespace BeeeOn {

class PEMMarkerStartToken : public Poco::Token {
public:
	PEMMarkerStartToken();

	bool start(char c, std::istream &in) override;
	void finish(std::istream &in) override;
	Class tokenClass() const override;

private:
	bool m_invalid;
};

class PEMMarkerToken : public Poco::Token {
public:
	PEMMarkerToken(const std::string &marker);

	bool start(char c, std::istream &in) override;
	void finish(std::istream &in) override;
	Class tokenClass() const override;

protected:
	bool finish(const std::string &expect,
			std::istream &in);

private:
	std::string m_expect;
	bool m_invalid;
};

class Base64Token : public Poco::Token {
public:
	Base64Token();

	bool start(char c, std::istream &in) override;
	void finish(std::istream &in) override;
	Class tokenClass() const override;

protected:
	bool isBase64(char c) const;

private:
	bool m_invalid;
};

}

PEMMarkerStartToken::PEMMarkerStartToken():
	m_invalid(false)
{
}

bool PEMMarkerStartToken::start(char c, std::istream &in)
{
	if (c != '-')
		return false;

	if (in.peek() != '-')
		return false;

	_value = c;
	m_invalid = false;
	return true;
}

void PEMMarkerStartToken::finish(istream &in)
{
	int c = in.peek();

	for (size_t i = 1; i < 5; ++i) {
		if (c != '-') {
			m_invalid = true;
			return;
		}

		_value += (char) c;
		in.get();
		c = in.peek();
	}
}

Token::Class PEMMarkerStartToken::tokenClass() const
{
	return m_invalid? Token::INVALID_TOKEN
			: Token::SPECIAL_COMMENT_TOKEN;
}

PEMMarkerToken::PEMMarkerToken(const std::string &marker):
	m_expect(marker),
	m_invalid(false)
{
}

bool PEMMarkerToken::start(char c, std::istream &in)
{
	if (c != m_expect[0])
		return false;

	if (in.peek() != m_expect[1])
		return false;

	_value = c;
	m_invalid = false;
	return true;
}

bool PEMMarkerToken::finish(const string &expect, istream &in)
{
	int c = in.peek();

	for (size_t i = 1; i < m_expect.size(); ++i) {
		if (c != m_expect[i])
			return true;

		_value += (char) c;
		in.get();
		c = in.peek();
	}

	return false;
}

void PEMMarkerToken::finish(std::istream &in)
{
	m_invalid = finish(m_expect, in);
}

Token::Class PEMMarkerToken::tokenClass() const
{
	return m_invalid? Token::INVALID_TOKEN
			: Token::USER_TOKEN;
}

Base64Token::Base64Token():
	m_invalid(false)
{
}

bool Base64Token::isBase64(char c) const
{
	if (c >= 'A' && c <= 'Z')
		return true;
	if (c >= 'a' && c <= 'z')
		return true;
	if (c >= '0' && c <= '9')
		return true;
	if (c == '+' || c == '/')
		return true;

	return false;
}

bool Base64Token::start(char c, istream &in)
{
	if (!isBase64(c))
		return false;

	if (!isBase64(in.peek()))
		return false;

	_value += c;
	m_invalid = false;
	return true;
}

void Base64Token::finish(istream &in)
{
	int c;

	c = in.peek();
	while (c != EOF) {
		if (c == '\n') {
			in.get();
			c = in.peek();
			continue;
		}

		if (isBase64(c) || c == '=')
			_value += c;
		else
			break;

		in.get();
		c = in.peek();
	}
}

Token::Class Base64Token::tokenClass() const
{
	return Token::STRING_LITERAL_TOKEN;
}

X509Fingerprint::X509Fingerprint(const X509Certificate *cert):
	m_cert(cert)
{
}

Poco::DigestEngine::Digest X509Fingerprint::digest(
		const string &algo)
{
	Poco::Crypto::DigestEngine engine(algo);
	performDigest(engine);
	return engine.digest();
}

string X509Fingerprint::digestToHex(const string &algo)
{
	Poco::Crypto::DigestEngine engine(algo);
	performDigest(engine);
	return Poco::DigestEngine::digestToHex(engine.digest());
}

#define PEM_HEADER "BEGIN CERTIFICATE-----\n"
#define PEM_FOOTER "END CERTIFICATE-----"

void X509Fingerprint::performDigest(Poco::DigestEngine &engine)
{
	stringstream ss;
	m_cert->save(ss);

	StreamTokenizer pemParser(ss);
	pemParser.addToken(new PEMMarkerStartToken());
	pemParser.addToken(new PEMMarkerToken(PEM_HEADER));
	pemParser.addToken(new PEMMarkerToken(PEM_FOOTER));
	pemParser.addToken(new Base64Token());
	pemParser.addToken(new WhitespaceToken());

	const Token *token;
	
	token = pemParser.next();
	if (!token->is(Token::SPECIAL_COMMENT_TOKEN))
		throw SyntaxException("missing start of PEM header");

	if (token->asString() != "-----")
		throw SyntaxException("expected start of PEM header");

	token = pemParser.next();
	if (!token->is(Token::USER_TOKEN))
		throw SyntaxException("missing PEM header");

	if (token->asString() != PEM_HEADER)
		throw SyntaxException("expected PEM header");

	token = pemParser.next();
	if (!token->is(Token::STRING_LITERAL_TOKEN))
		throw SyntaxException("missing PEM content");

	const string &text = token->asString();

	token = pemParser.next();
	if (!token->is(Token::SPECIAL_COMMENT_TOKEN))
		throw SyntaxException("missing start of PEM footer");

	if (token->asString() != "-----")
		throw SyntaxException("expected start of PEM footer");

	token = pemParser.next();
	if (!token->is(Token::USER_TOKEN))
		throw SyntaxException("missing PEM footer");

	if (token->asString() != PEM_FOOTER)
		throw SyntaxException("expected PEM footer");

	engine.update(Base64::decode(text));
}
