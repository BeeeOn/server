#pragma once

#include <list>

#include <Poco/SharedPtr.h>

#include "dao/FCMTokenDao.h"
#include "model/User.h"
#include "service/Single.h"

namespace BeeeOn {

/**
 * FCMTokenService class provides interface for unregistering and registering
 * users tokens. Every user that wants to receive notifications from BeeeOn
 * server is registered by an app with Firebase Cloud Messaging (FCM)
 * notification service. The FCM provides an authorization token to each
 * user. The received token is then registered with the BeeeOn server.
 * To cancel receiving of notifications from the BeeeOn server, the token
 * can be unregistered.
 *
 * @see https://firebase.google.com/docs/cloud-messaging/
 *
 */
class FCMTokenService {
public:
	typedef Poco::SharedPtr<FCMTokenService> Ptr;

	virtual ~FCMTokenService();

	/**
	 * Register the given token. The token contains its ID and user to
	 * register with.
	 */
	virtual void registerToken(Single<FCMToken> &input) = 0;

	/**
	 * Unregister the given token. The token contains its ID by which
	 * can be action done.
	 */
	virtual void unregisterToken(Single<FCMToken> &input) = 0;
};

}
