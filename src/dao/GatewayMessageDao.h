#pragma once

#include <vector>

#include <Poco/SharedPtr.h>

#include "model/Gateway.h"
#include "model/GatewayMessage.h"

namespace BeeeOn {

class GatewayMessageDao {
public:
	typedef Poco::SharedPtr<GatewayMessageDao> Ptr;

	virtual ~GatewayMessageDao();

	virtual bool insert(GatewayMessage &message, size_t limit) = 0;
	virtual bool remove(
		const GatewayMessage &message,
		const Gateway &gateway) = 0;
	virtual bool fetch(
		GatewayMessage &message,
		const Gateway &gateway) = 0;
	virtual void fetchBy(
		std::vector<GatewayMessage> &messages,
		const Gateway &gateway) = 0;
};

}
