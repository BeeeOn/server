#ifndef SERVER_GATEWAY_RPC_RESULT_H
#define SERVER_GATEWAY_RPC_RESULT_H

#include <Poco/AutoPtr.h>
#include <Poco/RefCountedObject.h>

namespace BeeeOn {

/**
 * Default RPC result, which do not hold any data.
 */
class GatewayRPCResult : public Poco::RefCountedObject {
public:
	typedef Poco::AutoPtr<GatewayRPCResult> Ptr;

	enum Status {
		PENDING,
		NOT_CONNECTED,
		TIMEOUT,
		ACCEPTED,
		SUCCESS,
		FAILED
	};

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
