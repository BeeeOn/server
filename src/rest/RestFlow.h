#ifndef BEEEON_REST_FLOW_H
#define BEEEON_REST_FLOW_H

#include <Poco/URI.h>

#include "l10n/Locale.h"
#include "l10n/Translator.h"
#include "rest/PocoContext.h"
#include "rest/UriPattern.h"
#include "server/Session.h"

namespace BeeeOn {

class RestLinker;

/**
 * Wraps information about a single Rest HTTP request. It provides
 * access to the request and response data, to the current session
 * (if the caller is logged in), the URI and extracted parameters.
 */
class RestFlow {
public:
	RestFlow(
		RestLinker &linker,
		Poco::URI &uri,
		UriPattern::Params &params,
		PocoRequest request,
		PocoResponse response);

	/**
	 * URI (HTTP path like /index.html?q=123) requested by the user.
	 */
	const Poco::URI &uri() const;

	/**
	 * Parameters extract from URI via UriPattern. This is used
	 * to decode which Rest resources are being accessed.
	 */
	const UriPattern::Params &params() const;

	/**
	 * Access URI parameters directly.
	 * @throws InvalidArgumentException when the kes is missing.
	 */
	std::string param(const std::string &key) const;

	/**
	 * Access URI parameters directly. If such param is not available,
	 * return value given as def.
	 */
	std::string param(const std::string &key, const std::string &def) const;

	/**
	 * Provides a linker that is intended for building links to
	 * other resources without knowing the addressing scheme.
	 */
	RestLinker &linker();

	/**
	 * Current request representation.
	 */
	const PocoRequest &request() const;
	PocoRequest &request();

	/**
	 * Current response representation.
	 */
	const PocoResponse &response() const;
	PocoResponse &response();

	/**
	 * @return true if this flow is associated with a session
	 */
	bool hasSession() const;

	/**
	 * Set session of this flow. This is usually done by some
	 * lower layer.
	 */
	void setSession(Session::Ptr session);

	/**
	 * Provide access to the current session. If there is no
	 * session associated with the current flow, it returns
	 * NULL.
	 */
	Session::Ptr session() const;

	/**
	 * Set localization translator for current flow.
	 */
	void setTranslator(Translator::Ptr translator);

	/**
	 * Access localization translator for current flow.
	 */
	Translator::Ptr translator() const;

	/**
	 * Set locale for the flow. If a session is set, than the session locale
	 * has higher priority.
	 */
	void setLocale(const Locale &locale);

	/**
	 * Obtain locale from the session if any. Otherwise, use the
	 * m_locale. The return value depends also on the given argument
	 * ignoreSessionLocale.
	 */
	const Locale &locale(bool ignoreSessionLocale = false) const;

private:
	RestLinker &m_linker;
	Poco::URI &m_uri;
	UriPattern::Params &m_params;
	PocoRequest m_request;
	PocoResponse m_response;
	Session::Ptr m_session;
	Translator::Ptr m_translator;
	Locale m_locale;
};

}

#endif
