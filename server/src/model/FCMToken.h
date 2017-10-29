#pragma once

#include "model/TokenID.h"
#include "model/User.h"

namespace BeeeOn {

/**
 * Class representation of authorization token of Firebase Cloud Messaging
 * notification service. Authorization tokens are used for granting authorization
 * to BeeeOn server from users to be able sending them notifications.
 */
class FCMToken : public Entity<TokenID> {
public:
	FCMToken();
	FCMToken(const ID &id);

	~FCMToken();

	User user() const;
	void setUser(const User &user);

private:
	User m_user;
};

typedef FCMToken::ID FCMTokenID;

}
