#pragma once

#include <map>
#include <set>

#include <Poco/Timespan.h>

#include "model/FCMToken.h"

namespace BeeeOn {

/**
 * @brief Representation of response coming from the Firebase Cloud Messaging server
 * as a result of sending a notification.
 *
 * @see https://developers.google.com/cloud-messaging/http-server-ref
 */
class FCMResponse {
public:
	/**
	 * The constant to express that there is no recommended delay.
	 */
	static const Poco::Timespan UNKNOWN_DELAY;

	FCMResponse();

	/**
	 * If the request is to be retried later, this property should contain
	 * a recommended delay to wait. The delay is valid only if non-negative.
	 * Use the constant UNKNOWN_DELAY to express an unknown delay.
	 */
	void setRetryAfter(const Poco::Timespan &delay);
	const Poco::Timespan &retryAfter() const;

	/**
	 * Unavailable recipients that we have to send the original
	 * notification again.
	 */
	void setUnderliveredRecipients(const std::set<FCMTokenID> &recipients);
	const std::set<FCMTokenID> &undeliveredRecipients() const;

	/**
	 * When a token to which the notification was sent is invalid,
	 * the FCM API reponses with recomendation of removing this token
	 * from database.
	 */
	void setFailingRecipients(const std::set<FCMTokenID> &recipients);
	const std::set<FCMTokenID> &failingRecipients() const;

	/**
	 * The FCM API can recommend to replace a token for certain
	 * recipients. The mapping tells which token should be replaced
	 * by which replacement.
	 */
	void setRenewedRecipients(const std::map<FCMTokenID, FCMTokenID> &recipients);
	const std::map<FCMTokenID, FCMTokenID> &renewedRecipients() const;

private:
	Poco::Timespan m_retryAfter;
	std::set<FCMTokenID> m_undelivered;
	std::set<FCMTokenID> m_failing;
	std::map<FCMTokenID, FCMTokenID> m_renewed;
};

}
