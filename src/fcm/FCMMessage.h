#pragma once

#include <set>
#include <string>

#include <Poco/Timespan.h>

#include "model/FCMToken.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief Provides representation of a notification suitable for sending to
 * the Firebase Cloud Messaging service.
 *
 * @see https://developers.google.com/cloud-messaging/http-server-ref
 * @see https://firebase.google.com/docs/cloud-messaging/concept-options
 */
class FCMMessage {
public:
	enum Priority {
		PRIORITY_NORM,
		PRIORITY_HIGH,
	};

	/**
	 * Maximal time to live in seconds supported by FCM API (28 days).
	 */
	static const Poco::Timespan MAX_SUPPORTED_TTL;

	FCMMessage();

	/**
	 * Adds one recipient into JSON array of recipients
	 */
	void addRecipient(const FCMTokenID &to);

	/**
	 * Sets recipients to array of defined recipients.
	 */
	void setRecipients(const std::set<FCMTokenID> &recipients);

	/**
	 * Returns recipients of the notification in JSON::Array
	 */
	const std::set<FCMTokenID> &recipients() const;

	/**
	 * Title of the notification's text part.
	 */
	void setTitle(const std::string &title);
	const std::string &title() const;

	/**
	 * Body of the notification's text part.
	 */
	void setBody(const std::string &body);
	const std::string &body() const;

	/**
	 * time_to_live from FCM documentation:
	 *
	 * - This parameter specifies how long (in seconds) the message should be kept in FCM storage if
	 *   the device is offline.
	 *
	 * - The maximum time to live supported is 4 weeks, and the default value is 4 weeks when not set.
	 *
	 * - TTL of 0 requires an immediate delivery that is however not guaranteed and such messages
	 *   can be discarded.
	 */
	void setTimeToLive(const Poco::Timespan &timeToLive);
	const Poco::Timespan &timeToLive() const;

	/**
	 * priority from FCM documentation:
	 *
	 * - Sets the priority of the message. Valid values are "normal" and "high."
	 *
	 * - On iOS, these correspond to APNs priorities 5 and 10. By default, notification messages are
	 *   sent with high priority, and data messages are sent with normal priority.
	 *
	 * - Normal priority optimizes the client app's battery consumption and should be used unless
	 *   immediate delivery is required.
	 *
	 * - For messages with normal priority, the app may receive the message with unspecified delay.
	 *
	 * - When a message is sent with high priority, it is sent immediately, and the app can wake
	 *   a sleeping device and open a network connection to your server.
	 */
	void setPriority(const Priority &priority);
	Priority priority() const;

	/**
	 * content_available from FCM documentation:
	 *
	 *  - On iOS, use this field to represent content-available in the APNs payload.
	 *
	 *  - When a notification or message is sent and this is set to true, an inactive client app is awoken.
	 *
	 *  - On Android, data messages wake the app by default. On Chrome, currently not supported.
	 */
	void setContentAvailable(bool contentAvailable);
	bool contentAvailable() const;

private:
	std::set<FCMTokenID> m_recipients;
	std::string m_title;
	std::string m_body;
	Poco::Timespan m_timeToLive;
	Priority m_priority;
	bool m_contentAvailable;
};

}
