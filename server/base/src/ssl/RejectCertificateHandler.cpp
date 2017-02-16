#include <Poco/Logger.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Net/CertificateHandlerFactory.h>
#include <Poco/Net/VerificationErrorArgs.h>

#include "ssl/RejectCertificateHandler.h"
#include "ssl/X509Fingerprint.h"

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

BetterRejectCertificateHandler::BetterRejectCertificateHandler(bool server):
	InvalidCertificateHandler(server)
{
}

void BetterRejectCertificateHandler::onInvalidCertificate(
		const void *sender,
		VerificationErrorArgs &error)
{
	const X509Certificate &cert = error.certificate();

	error.setIgnoreError(false);

	logger().warning(
		"rejecting certificate:\n"
		+ format("Name: %s\nIssuer: %s\nSubject: %s\n"
			"Valid from: %s\nSHA256: %s\ndue to: %s",
			cert.commonName(),
			cert.issuerName(),
			cert.subjectName(),
			DateTimeFormatter::format(cert.validFrom(),
				DateTimeFormat::SORTABLE_FORMAT),
		X509Fingerprint(&cert).digestToHex("SHA256"),
		error.errorMessage()),
		__FILE__, __LINE__);
}

POCO_REGISTER_CHFACTORY(BeeeOn, BetterRejectCertificateHandler)
