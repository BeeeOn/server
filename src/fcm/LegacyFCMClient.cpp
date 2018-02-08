#include <Poco/DateTimeParser.h>
#include <Poco/Logger.h>
#include <Poco/NumberParser.h>
#include <Poco/String.h>
#include <Poco/URI.h>

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/Object.h>

#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "fcm/LegacyFCMClient.h"
#include "net/HTTPUtil.h"
#include "ssl/SSLClient.h"
#include "util/JsonUtil.h"
#include "util/Sanitize.h"

using namespace BeeeOn;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Net;
using namespace std;

BEEEON_OBJECT_BEGIN(BeeeOn, LegacyFCMClient)
BEEEON_OBJECT_CASTABLE(FCMClient)
BEEEON_OBJECT_TEXT("fcmApiUri", &LegacyFCMClient::setApiUri)
BEEEON_OBJECT_REF("sslConfig", &LegacyFCMClient::setSSLConfig)
BEEEON_OBJECT_END(BeeeOn, LegacyFCMClient)

LegacyFCMClient::LegacyFCMClient():
	m_apiUri("https://fcm.googleapis.com/fcm/send")
{
}

FCMResponse LegacyFCMClient::sendMessage(
		const FCMMessage &message,
		const string &serverKey,
		bool dryRun)
{
	const URI uri(m_apiUri);

	const string &content = buildContent(message, dryRun);

	HTTPRequest req(HTTPMessage::HTTP_1_1);
	req.setMethod(HTTPRequest::HTTP_POST);
	req.setContentType("application/json");
	req.set("Authorization", "key=" + serverKey);
	req.set("Accept", "application/json");
	req.setContentLength(content.length());

	if (logger().trace()) {
		ostringstream s;
		req.write(s);
		s << content;
		logger().trace("request: " + s.str(), __FILE__, __LINE__);
	}

	HTTPEntireResponse response = HTTPUtil::makeRequest(
		req, uri, content, m_sslConfig);

	if (response.getStatus() >= HTTPEntireResponse::HTTP_BAD_REQUEST) {
		logger().error(
			"status: " + to_string(response.getStatus()),
			__FILE__, __LINE__);
	}
	else if (logger().debug()) {
		logger().information(
			"status: " + to_string(response.getStatus()),
			__FILE__, __LINE__);
	}

	if (logger().debug()) {
		logger().debug("response: " + response.getBody(),
				__FILE__, __LINE__);
	}

	return processResponse(message, response);
}

string LegacyFCMClient::buildContent(const FCMMessage &message, bool dryRun) const
{
	Object::Ptr notification = new Object;

	switch (message.priority()) {
	case FCMMessage::PRIORITY_NORM:
		notification->set("priority", "normal");
		break;
	case FCMMessage::PRIORITY_HIGH:
		notification->set("priority", "high");
		break;
	default:
		throw IllegalStateException("invalid FCM notification priority: "
				+ to_string(message.priority()));
	}

	if (message.recipients().empty())
		throw IllegalStateException("no recipient in FCM notification");

	Array::Ptr recipients = new Array;
	for (const auto &recipient : message.recipients())
		recipients->add(recipient.toString());

	notification->set("registration_ids", recipients);

	notification->set("time_to_live", message.timeToLive().totalSeconds());
	notification->set("content_available", message.contentAvailable());

	if (dryRun)
		notification->set("dry_run", true);

	Object::Ptr text = new Object;

	if (message.title().empty())
		throw IllegalStateException("missing title of FCM notification");

	text->set("title", message.title());

	if (message.body().empty())
		throw IllegalStateException("missing text of FCM notification");

	text->set("body", message.body());
	notification->set("notification", text);

	return Dynamic::Var::toString(notification);
}

Timespan LegacyFCMClient::parseRetryAfter(
		string content,
		const Poco::Timestamp &now) const
{
	int seconds = 0;

	trimInPlace(content);

	if (NumberParser::tryParse(content, seconds)) {
		if (seconds < 0)
			return FCMResponse::UNKNOWN_DELAY;

		return seconds * Timespan::SECONDS;
	}

	DateTime since;
	int timeZone = 0;

	if (DateTimeParser::tryParse(content, since, timeZone)) {
		since.makeUTC(timeZone);
		const Timespan &diff = since.timestamp() - now;

		if (diff < 0)
			return FCMResponse::UNKNOWN_DELAY;

		return diff;
	}

	logger().warning("failed to parse Retry-After",
			__FILE__, __LINE__);

	return FCMResponse::UNKNOWN_DELAY;
}

FCMResponse LegacyFCMClient::processResponse(
		const FCMMessage &message,
		const HTTPEntireResponse &httpResponse) const
{
	FCMResponse response;

	// process the header, it is suitable for various error conditions below
	if (httpResponse.has("Retry-After"))
		response.setRetryAfter(parseRetryAfter(httpResponse["Retry-After"]));

	if (httpResponse.getStatus() >= HTTPEntireResponse::HTTP_INTERNAL_SERVER_ERROR) {
		response.setUnderliveredRecipients(message.recipients());
		return response;
	}

	if (httpResponse.getStatus() == HTTPEntireResponse::HTTP_UNAUTHORIZED)
		throw NotAuthenticatedException("failed to authenticate to FCM server");

	if (httpResponse.getStatus() >= HTTPEntireResponse::HTTP_BAD_REQUEST)
		throw ProtocolException("invalid body for FCM server");

	return processResponseBody(
		message,
		response,
		JsonUtil::parse(httpResponse.getBody())
	);
}

FCMResponse LegacyFCMClient::processResponseBody(
		const FCMMessage &message,
		FCMResponse &response,
		Object::Ptr object) const
{
	const int failure = object->getValue<int>("failure");
	const int canonicalIds = object->getValue<int>("canonical_ids");

	if (logger().debug()) {
		const int success = object->getValue<int>("success");
		const Array::Ptr results = object->getArray("results");

		logger().debug("response "
				+ to_string(success) + "/"
				+ to_string(failure) + "/"
				+ to_string(canonicalIds) + "/"
				+ to_string(results->size()),
				__FILE__, __LINE__);
	}

	if (failure == 0 && canonicalIds == 0)
		return response;

	return processAffectedTokens(message, response, object);
}

FCMResponse LegacyFCMClient::processAffectedTokens(
		const FCMMessage &message,
		FCMResponse &response,
		Object::Ptr object) const
{
	const Array::Ptr results = object->getArray("results");

	set<FCMTokenID> undelivered;
	set<FCMTokenID> failing;
	map<FCMTokenID, FCMTokenID> renewed;

	unsigned int index = 0;

	for (const auto &original : message.recipients()) {
		if (index >= results->size())
			break;

		const Object::Ptr result = results->getObject(index);

		index += 1;

		if (result->has("error")) {
			const string &spec = result->getValue<string>("error");

			if (isUndelivered(spec)) {
				undelivered.emplace(original);
			}
			else if (isFailing(spec)) {
				failing.emplace(original);
			}
			else if (isRateExceeded(spec)) {
				logger().critical(
					"exceeded message rate",
					__FILE__, __LINE__);
			}
			else if (isSenderMismatch(spec)) {
				logger().critical(
					"senderId mismatch, check configuration",
					__FILE__, __LINE__);
				failing.emplace(original);
			}
			else {
				logger().warning(
					"unhandled error type '" + spec + "'",
					__FILE__, __LINE__);
			}
		}
		else if (result->has("message_id") && result->has("registration_id")) {
			const string &messageId = result->getValue<string>("message_id");
			const string &renew = result->getValue<string>("registration_id");

			if (logger().debug()) {
				logger().debug("renew user token for " + messageId,
						__FILE__, __LINE__);
			}

			try {
				FCMTokenID tokenRenew = FCMTokenID::parse(Sanitize::token(renew));
				renewed.emplace(original, tokenRenew);
			}
			catch (const Exception &e) {
				logger().log(e, __FILE__, __LINE__);
			}
		}
		else if (result->has("message_id")) {
			const string &messageId = result->getValue<string>("message_id");

			if (logger().debug()) {
				logger().debug("no reaction for " + messageId,
						__FILE__, __LINE__);
			}
		}
		else {
			logger().warning("unrecognized response", __FILE__, __LINE__);
		}
	}

	response.setUnderliveredRecipients(undelivered);
	response.setFailingRecipients(failing);
	response.setRenewedRecipients(renewed);

	return response;
}

bool LegacyFCMClient::isUndelivered(const string &spec) const
{
	return spec == "Unavailable";
}

bool LegacyFCMClient::isFailing(const string &spec) const
{
	return spec == "InvalidRegistration" || spec == "NotRegistered";
}

bool LegacyFCMClient::isRateExceeded(const string &spec) const
{
	return spec == "DeviceMessageRateExceeded" || spec == "TopicsMessageRateExceeded";
}

bool LegacyFCMClient::isSenderMismatch(const string &spec) const
{
	return spec == "MismatchSenderId";
}

void LegacyFCMClient::setApiUri(const string &apiUri)
{
	m_apiUri = apiUri;
}

void LegacyFCMClient::setSSLConfig(SharedPtr<SSLClient> sslConfig)
{
	m_sslConfig = sslConfig;
}
