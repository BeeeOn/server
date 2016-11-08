#ifndef BEEEON_X509_FINGERPRINT_H
#define BEEEON_X509_FINGERPRINT_H

#include <Poco/DigestEngine.h>

namespace Poco {
namespace Crypto {

class X509Certificate;

}
}

namespace BeeeOn {

class X509Fingerprint {
public:
	X509Fingerprint(const Poco::Crypto::X509Certificate *cert);

	Poco::DigestEngine::Digest digest(const std::string &algo);
	std::string digestToHex(const std::string &algo);

protected:
	void performDigest(Poco::DigestEngine &engine);

private:
	const Poco::Crypto::X509Certificate *m_cert;
};

}

#endif
