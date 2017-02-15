#ifndef BEEEON_REJECT_CERTIFICATE_HANDLER_H
#define BEEEON_REJECT_CERTIFICATE_HANDLER_H

#include <Poco/Net/InvalidCertificateHandler.h>

#include "util/Loggable.h"

namespace Poco {

namespace Net {

class VerificationErrorArgs;

}
}

namespace BeeeOn {

class BetterRejectCertificateHandler :
	public Poco::Net::InvalidCertificateHandler,
	public Loggable {
public:
	BetterRejectCertificateHandler(bool server);

	void onInvalidCertificate(const void *sender,
		Poco::Net::VerificationErrorArgs &errorCert) override;
};

}

#endif
