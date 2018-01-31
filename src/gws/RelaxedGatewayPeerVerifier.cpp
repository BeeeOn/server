#include <string>

#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "gws/RelaxedGatewayPeerVerifier.h"
#include "ssl/X509Fingerprint.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RelaxedGatewayPeerVerifierFactory)
BEEEON_OBJECT_CASTABLE(SocketGatewayPeerVerifierFactory)
BEEEON_OBJECT_END(BeeeOn, RelaxedGatewayPeerVerifierFactory)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

RelaxedGatewayPeerVerifier::RelaxedGatewayPeerVerifier(
		SharedPtr<X509Certificate> peerCertificate):
	m_peerCertificate(peerCertificate)
{
}

void RelaxedGatewayPeerVerifier::verifyPeer(const Gateway &gateway) const
{
	if (m_peerCertificate.isNull()) {
		logger().information(
			"gateway " + gateway.id().toString()
			+ " accepted, however it does not provide any peer certificate",
			__FILE__, __LINE__);
		return;
	}

	const string &cn = m_peerCertificate->commonName();
	if (cn != gateway.id().toString()) {
		logger().warning(
			"gateway " + gateway.id().toString()
			+ " does not match the peer certificate's CN: "
			+ cn,
			__FILE__, __LINE__);
	}
	else {
		logger().information(
			"gateway " + gateway.id().toString()
			+ " was successfully verified",
			__FILE__, __LINE__);
	}
}

SharedPtr<X509Certificate> RelaxedGatewayPeerVerifierFactory::preverify(
		const StreamSocket &socket) const
{
	SharedPtr<X509Certificate> certificate = extractPeerCert(socket);

	if (certificate.isNull()) {
		logger().warning(
			"no peer certificate for "
			+ socket.peerAddress().toString(),
			__FILE__, __LINE__);
	}
	else {
		const auto expiresOnString = DateTimeFormatter::format(
			certificate->expiresOn(), DateTimeFormat::ISO8601_FORMAT);

		const DateTime now;

		if (certificate->expiresOn() < now) {
			logger().warning(
				"peer certificate from "
				+ socket.peerAddress().toString()
				+ " seems to be expired: "
				+ expiresOnString,
				__FILE__, __LINE__);
		}

		logger().information(
			"peer from " + socket.peerAddress().toString()
			+ " provides certificate:\n"
			+ "CN: " + certificate->commonName() + "\n"
			+ "Issuer: " + certificate->issuerName() + "\n"
			+ "Subject: " + certificate->subjectName() + "\n"
			+ "Valid from: " + DateTimeFormatter::format(
				certificate->validFrom(), DateTimeFormat::ISO8601_FORMAT) + "\n"
			+ "Expires on: " + expiresOnString + "\n"
			+ "SHA256: " + X509Fingerprint(certificate).digestToHex("SHA256"),
			__FILE__, __LINE__);
	}

	return certificate;
}

GatewayPeerVerifier::Ptr RelaxedGatewayPeerVerifierFactory::preverifyAndCreate(
		const StreamSocket &socket)
{
	SharedPtr<X509Certificate> certificate;

	if (!socket.secure()) {
		logger().information(
			"no gateway verification will occur for "
			+ socket.peerAddress().toString()
			+ " due to using a non-SSL/TLS socket",
			__FILE__, __LINE__);
	}
	else {
		certificate = preverify(socket);
	}

	return new RelaxedGatewayPeerVerifier(certificate);
}
