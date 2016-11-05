#ifndef BEEEON_SSL_CLIENT_H
#define BEEEON_SSL_CLIENT_H

#include "util/SSLFacility.h"

namespace BeeeOn {

/**
 * SSL Client context provider. Each group of SSL connections
 * (connection to certain server, connections by certain L7
 * protocol, etc.) would usually use an instance of the SSLClient.
 * Some applications may share a single instance.
 */
class SSLClient : public SSLFacility {
public:
	SSLClient();
	~SSLClient();

protected:
	Poco::Net::Context::Ptr createContext() override;
};

}

#endif
