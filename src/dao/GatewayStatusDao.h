#pragma once

#include <Poco/SharedPtr.h>

#include "model/Gateway.h"
#include "model/GatewayStatus.h"

namespace BeeeOn {

class GatewayStatusDao {
public:
	typedef Poco::SharedPtr<GatewayStatusDao> Ptr;

	virtual ~GatewayStatusDao();

	virtual bool insert(const GatewayStatus &status, const Gateway &gateway) = 0;
};

}
