#pragma once

#include <string>

#include <Poco/URI.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/HTMLForm.h>

#include "net/HTTPEntireResponse.h"
#include "ssl/SSLClient.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief HTTPClient simplifies construction of HTTP clients that always
 * requests (nearly) the same URL address.
 */
class HTTPClient :
	protected virtual Loggable {
public:
	HTTPClient(
		const std::string &method,
		const Poco::URI &url);
	HTTPClient(
		const std::string &method,
		const std::string &url);

	void setMethod(const std::string &method);
	void setURL(const Poco::URI &url);
	void setSSLConfig(SSLClient::Ptr config);
	void setEnforceSecure(bool enforce);

	/**
	 * @brief Make HTTP request to the preconfigured URL with the given
	 * HTTPForm instance applied to the request.
	 */
	HTTPEntireResponse request(Poco::Net::HTMLForm &form) const;

	/**
	 * @brief Make HTTP request to the preconfigured URL but with custom
	 * path and query parts. The given HTTPForm instance is applied to
	 * the request.
	 */
	HTTPEntireResponse request(
		const std::string &pathAndQuery,
		Poco::Net::HTMLForm &form) const;

	/**
	 * @brief In case, when the HTTP response meta-data is not important,
	 * this method can be used to access the response body directly.
	 * The HTTP request is performed with the preconfigured URL.
	 */
	std::string requestBody(Poco::Net::HTMLForm &form) const;

	/**
	 * @brief In case, when the HTTP response meta-data is not important,
	 * this method can be used to access the response body directly.
	 * The HTTP request is performed with the preconfigured URL but with
	 * custom path and query parts.
	 */
	std::string requestBody(
		const std::string &pathAndQuery,
		Poco::Net::HTMLForm &form) const;

	/**
	 * @brief Extract body from the given response. The method is useful
	 * to unify HTTP response processing as it performs logging of the
	 * response contents.
	 */
	std::string responseBody(const HTTPEntireResponse &response) const;

private:
	std::string m_method;
	Poco::URI m_url;
	SSLClient::Ptr m_sslConfig;
	bool m_enforceSecure;
};

}
