#pragma once

#include <vector>

#include <Poco/SharedPtr.h>

#include "model/FCMToken.h"
#include "model/Gateway.h"

namespace BeeeOn {

class FCMRecipientsDao {
public:
	typedef Poco::SharedPtr<FCMRecipientsDao> Ptr;

	virtual ~FCMRecipientsDao();

	virtual void fetchBy(std::vector<FCMToken> &tokens, const Gateway &gateway) = 0;
};

}
