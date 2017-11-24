#pragma once

#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>
#include <Poco/URI.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPSClientSession.h>

#include "fcm/FCMClient.h"
#include "fcm/FCMMessage.h"
#include "fcm/FCMResponse.h"
#include "net/HTTPEntireResponse.h"
#include "util/Loggable.h"

namespace BeeeOn {

class SSLClient;

/**
 * @brief LegacyFCMClient is a reusable thread-safe implementation of a client to
 * talk to the Firebase Cloud Messaging.
 *
 * The LegacyFCMClient implements the legacy HTTP(s) communication protocol when
 * talking to the FCM server. To perform a valid request, each notification must be
 * authorized via the server key obtained from the Firebase developers console
 * (legacy key of the cloud messaging service).
 */
class LegacyFCMClient : public FCMClient, protected Loggable {
public:
	typedef Poco::SharedPtr<LegacyFCMClient> Ptr;

	LegacyFCMClient();

	/**
	 * Sends the given notification to FCM server with a specific server key
	 * authorized to send notifications.
	 */
	FCMResponse sendMessage(
			const FCMMessage &message,
			const std::string &serverKey,
			bool dryRun = false);

	void setApiUri(const std::string &apiUri);
	void setSSLConfig(Poco::SharedPtr<SSLClient> sslConfig);

protected:
	/**
	 * Convert the notification to JSON Object as expected by the Firebase Cloud
	 * Messaging service.
	 *
	 * @see https://developers.google.com/cloud-messaging/http-server-ref
	 */
	std::string buildContent(const FCMMessage &message, bool dryRun) const;

	/**
	 * Process HTTP status and then parse the response body. Here, the Retry-After is
	 * extracted if present. If a HTTP status represents a client error, an exception
	 * is thrown.
	 *
	 * HTTP status 5xx returns a response with all original senders set as undelivered.
	 * Such message should be retried later.
	 *
	 * @throws Poco::Net::NotAuthenticatedException on status 401
	 * @throws Poco::ProtocolException on status 4xx
	 */
	FCMResponse processResponse(
			const FCMMessage &message,
			const HTTPEntireResponse &response) const;

	/**
	 * Detect errors or token renews and if any, process them.
	 * Otherwise, such response denotes tha that the message was
	 * successfully delivered and no more processing is done.
	 */
	FCMResponse processResponseBody(
			const FCMMessage &message,
			FCMResponse &response,
			Poco::JSON::Object::Ptr object) const;

	/**
	 * Process all response errors and token renews.
	 */
	FCMResponse processAffectedTokens(
			const FCMMessage &message,
			FCMResponse &response,
			Poco::JSON::Object::Ptr object) const;

	/**
	 * Connect to the Firebase Cloud service via HTTP or HTTPS.
	 */
	Poco::SharedPtr<Poco::Net::HTTPClientSession> connect(const Poco::URI &uri);

	/**
	 * Parse the header Retry-After that can come in an HTTP response.
	 * The Retry-After can be expressed either as seconds or as a date time.
	 *
	 * @param content - value of the Retry-After header
	 * @param now - reference time, can be omitted to use the current time
	 *
	 * @see https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Retry-After
	 */
	Poco::Timespan parseRetryAfter(
			std::string content,
			const Poco::Timestamp &now = {}) const;

	bool isUndelivered(const std::string &spec) const;
	bool isFailing(const std::string &spec) const;
	bool isRateExceeded(const std::string &spec) const;
	bool isSenderMismatch(const std::string &spec) const;

private:
	/**
	 * Uri of FCM server recieving notifications
	 */
	std::string m_apiUri;

	/**
	 * SSL configuration
	 */
	Poco::SharedPtr<SSLClient> m_sslConfig;
};
}
