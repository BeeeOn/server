#ifndef BEEEON_SSL_SERVER_H
#define BEEEON_SSL_SERVER_H

#include "util/SSLFacility.h"

namespace BeeeOn {

class SSLServer : public SSLFacility {
public:
	SSLServer();
	~SSLServer();

protected:
	Poco::Net::Context::Ptr createContext() override;
};

}

#endif
