#pragma once

#include <vector>
#include <Poco/SharedPtr.h>

#include "model/FCMToken.h"
#include "model/User.h"

namespace BeeeOn {

class FCMTokenDao {
public:
	typedef Poco::SharedPtr<FCMTokenDao> Ptr;

	virtual ~FCMTokenDao();

	virtual void insert(FCMToken &token) = 0;
	virtual void fetchBy(std::vector<FCMToken> &token, const User &user) = 0;
	virtual bool fetch(FCMToken &token) = 0;
	virtual bool replace(const FCMToken &tokenFrom, FCMToken &tokenTo) = 0;
	virtual bool remove(FCMToken &token) = 0;
};
}
