#include <Poco/Logger.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Net/CertificateHandlerFactory.h>
#include <Poco/Net/VerificationErrorArgs.h>

#include "util/RejectCertificateHandler.h"
#include "util/X509Fingerprint.h"
#include "Debug.h"

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

BetterRejectCertificateHandler::BetterRejectCertificateHandler(bool server):
	InvalidCertificateHandler(server),
	m_logger(LOGGER_CLASS(this))
{
}

void BetterRejectCertificateHandler::onInvalidCertificate(
		const void *sender,
		VerificationErrorArgs &error)
{
	const X509Certificate &cert = error.certificate();

	error.setIgnoreError(false);

	m_logger.warning(
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
