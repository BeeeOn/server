#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Logger.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/SecureStreamSocketImpl.h>
#include <Poco/Net/SSLException.h>

#include "di/Injectable.h"
#include "gws/X509GatewayPeerVerifier.h"
#include "ssl/X509Fingerprint.h"

BEEEON_OBJECT_BEGIN(BeeeOn, X509GatewayPeerVerifierFactory)
BEEEON_OBJECT_CASTABLE(SocketGatewayPeerVerifierFactory)
BEEEON_OBJECT_END(BeeeOn, X509GatewayPeerVerifierFactory)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

X509GatewayPeerVerifier::X509GatewayPeerVerifier(
		const X509Certificate &cert):
	m_peerCertificate(cert)
{
}

const X509Certificate &X509GatewayPeerVerifier::certificate() const
{
	return m_peerCertificate;
}

GatewayID X509GatewayPeerVerifier::extractID() const
{
	return GatewayID::parse(m_peerCertificate.commonName());
}

string X509GatewayPeerVerifier::formatDT(const DateTime &dt)
{
	return DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FORMAT);
}

string X509GatewayPeerVerifier::formatCert(const X509Certificate &cert)
{
	return "CN: " + cert.commonName() + "\n"
		+ "Issuer: " + cert.issuerName() + "\n"
		+ "Subject: " + cert.subjectName() + "\n"
		+ "Valid from: " + formatDT(cert.validFrom()) + "\n"
		+ "Expires on: " + formatDT(cert.expiresOn()) + "\n"
		+ "SHA256: " + X509Fingerprint(&cert).digestToHex("SHA256");
}

void X509GatewayPeerVerifier::verifyPeer(const Gateway &gateway) const
{
	if (logger().debug()) {
		logger().debug("verifying certificate:\n"
				+ formatCert(certificate()),
				__FILE__, __LINE__);
	}

	const auto &id = extractID();
	if (id != gateway.id()) {
		throw CertificateValidationException(
			"ID "
			+ id.toString()
			+ " does not match expected "
			+ gateway.id().toString());
	}
}

GatewayPeerVerifier::Ptr X509GatewayPeerVerifierFactory::preverifyAndCreate(
		const StreamSocket &socket)
{
	if (!socket.secure()) {
		throw InvalidSocketException(
			"socket "
			+ socket.peerAddress().toString()
			+ " is not secure");
	}

	SecureStreamSocketImpl *impl =
		dynamic_cast<SecureStreamSocketImpl *>(socket.impl());
	if (!impl) {
		throw InvalidSocketException(
			"socket impl is not SecureStreamSocketImpl");
	}

	if (!impl->havePeerCertificate()) {
		throw InvalidCertificateException(
			"no peer certificate for "
			+ socket.peerAddress().toString());
	}

	const X509Certificate &cert = impl->peerCertificate();

	const DateTime now;

	try {
		if (cert.expiresOn() < now) {
			throw InvalidCertificateException(
				"certificate has expired on: "
				+ X509GatewayPeerVerifier::formatDT(cert.expiresOn()));
		}

		if (cert.validFrom() > now) {
			throw InvalidCertificateException(
				"certificate is not valid yet: "
				+ X509GatewayPeerVerifier::formatDT(cert.validFrom()));
		}
	}
	catch (const Exception &e) {
		logger().error("failing peer certificate:\n"
				+ X509GatewayPeerVerifier::formatCert(cert),
				__FILE__, __LINE__);
		e.rethrow();
	}

	return new X509GatewayPeerVerifier(cert);
}
