#ifndef BEEEON_TESTING_HTTP_SERVER_RESPONSE_H
#define BEEEON_TESTING_HTTP_SERVER_RESPONSE_H

#include <sstream>

#include <Poco/Net/HTTPServerResponse.h>

namespace BeeeOn {

class TestingHTTPServerResponse : public Poco::Net::HTTPServerResponse {
public:
	TestingHTTPServerResponse();

	void sendContinue() override;
	bool continueSent() const;

	std::ostream &send() override;
	std::ostringstream &stringstream();

	void sendFile(const std::string &path, const std::string &mediaType) override;
	void sendBuffer(const void *buffer, std::size_t length) override;

	void redirect(const std::string &uri, HTTPStatus status = HTTP_FOUND) override;
	std::string redirectURI() const;
	HTTPStatus redirectStatus() const;
	bool redirected() const;

	void requireAuthentication(const std::string &realm) override;
	bool authenticationRequired() const;
	std::string authenticationRealm() const;

	bool sent() const override;

private:
	bool m_continueSent;
	std::string m_redirectURI;
	HTTPStatus m_redirectStatus;
	bool m_redirected;
	std::string m_authenticationRealm;
	bool m_authenticationRequired;
	std::ostringstream m_stream;
};

}

#endif
