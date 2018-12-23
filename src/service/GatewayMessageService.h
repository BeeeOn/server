#pragma once

#include <vector>

#include <Poco/SharedPtr.h>

#include "model/GatewayMessage.h"
#include "service/Relation.h"

namespace BeeeOn {

class GatewayMessageService {
public:
	typedef Poco::SharedPtr<GatewayMessageService> Ptr;

	virtual ~GatewayMessageService();

	virtual bool fetch(Relation<GatewayMessage, Gateway> &input) = 0;
	virtual void fetchMany(Relation<std::vector<GatewayMessage>, Gateway> &input) = 0;
	virtual bool remove(Relation<GatewayMessage, Gateway> &input) = 0;
};

}
