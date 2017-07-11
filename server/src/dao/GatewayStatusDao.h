#ifndef SERVER_GATEWAY_STATUS_DAO_H
#define SERVER_GATEWAY_STATUS_DAO_H

#include <Poco/SharedPtr.h>

#include "model/Gateway.h"

namespace BeeeOn {

class GatewayStatusDao {
public:
	typedef Poco::SharedPtr<GatewayStatusDao> Ptr;

	virtual bool insert(Gateway &gateway) = 0;
};

}

#endif
