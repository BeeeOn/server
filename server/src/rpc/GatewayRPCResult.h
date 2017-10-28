#ifndef SERVER_GATEWAY_RPC_RESULT_H
#define SERVER_GATEWAY_RPC_RESULT_H

#include <string>

#include <Poco/AutoPtr.h>
#include <Poco/RefCountedObject.h>

#include "util/Enum.h"

namespace BeeeOn {

/**
 * Default RPC result, which do not hold any data.
 */
class GatewayRPCResult : public Poco::RefCountedObject {
public:
	typedef Poco::AutoPtr<GatewayRPCResult> Ptr;

	struct StatusEnum {
		enum Raw {
			PENDING,
			NOT_CONNECTED,
			TIMEOUT,
			ACCEPTED,
			SUCCESS,
			FAILED
		};

		static EnumHelper<Raw>::ValueMap &valueMap();
	};

	typedef Enum<GatewayRPCResult::StatusEnum> Status;

	GatewayRPCResult();

	void setStatus(const Status status);
	Status status() const;

	static Status convert(const int value);

protected:
	~GatewayRPCResult();

private:
	Status m_status;
};

}

#endif
