#ifndef SERVER_GATEWAY_STATUS_DAO_H
#define SERVER_GATEWAY_STATUS_DAO_H

#include <Poco/SharedPtr.h>

#include "model/Gateway.h"
#include "model/GatewayStatus.h"

namespace BeeeOn {

class GatewayStatusDao {
public:
	typedef Poco::SharedPtr<GatewayStatusDao> Ptr;

	virtual bool insert(const GatewayStatus &status, const Gateway &gateway) = 0;
};

}

#endif
