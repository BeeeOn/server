#include <Poco/Exception.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Logger.h>

#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

#include "l10n/HTTPLocaleExtractor.h"
#include "rest/PocoRestRequestHandler.h"
#include "rest/RestFlow.h"
#include "rest/RestRouter.h"
#include "rest/RestLinker.h"
#include "server/SessionVerifier.h"
#include "util/Sanitize.h"
#include "util/ThreadNamer.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

PocoRestRequestHandler::PocoRestRequestHandler(
		RestLinker &linker,
		TranslatorFactory &factory,
		HTTPLocaleExtractor &localeExtractor,
		HTTPFilterChain &filterChain):
	m_linker(linker),
	m_translatorFactory(factory),
	m_localeExtractor(localeExtractor),
	m_filterChain(filterChain)
{
}

void PocoRestRequestHandler::setStarted(const Clock &started)
{
	m_started = started;
}

const Clock &PocoRestRequestHandler::started() const
{
	return m_started;
}

void PocoRestRequestHandler::setAction(RestAction::Ptr action)
{
	m_action = action;
}

void PocoRestRequestHandler::setActionParams(
		const MappedRestAction::Params &params)
{
	m_params = params;
}

void PocoRestRequestHandler::setSession(Session::Ptr session)
{
	m_session = session;
}

bool PocoRestRequestHandler::expectedContentLength()
{
	const string &method = request().getMethod();

	if (method != "POST" && method != "PUT" && method != "PATCH")
		return true;

	if (!request().hasContentLength()) {
		response().setStatusAndReason(HTTPResponse::HTTP_LENGTH_REQUIRED);
		return false;
	}

	int inputMaxSize = m_action->inputMaxSize();
	if (inputMaxSize < 0)
		return true;

	auto contentLength = request().getContentLength();
	if (contentLength > inputMaxSize) {
		if (logger().warning()) {
			logger().warning(
				"too long input: "
				+ to_string(contentLength)
				+ " for "
				+ m_action->fullName(),
				__FILE__, __LINE__);
		}

		response().setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
		return false;
	}

	return true;
}

string PocoRestRequestHandler::asString(const MappedRestAction::Params &params) const
{
	string s("[");
	auto it = params.begin();

	for (; it != params.end(); ++it) {
		if (it != params.begin())
			s.append(", ");

		s.append(it->first);
		s.append(" => ");
		s.append(it->second);
	}

	s.append("]");
	return s;
}

void PocoRestRequestHandler::prepareInternalAction(
		const RestAction::Ptr action)
{
	response().set("Cache-Control", "public, no-cache");
}

void PocoRestRequestHandler::prepareMappedAction(
		const MappedRestAction::Ptr action)
{
	if (action->caching() == 0) {
		response().set("Cache-Control", "public, no-cache");
	}
	else if (action->caching() > 0) {
		const Timespan shift(action->caching(), 0);
		const DateTime now;

		response().set("Expires",
			DateTimeFormatter::format(now + shift, DateTimeFormat::HTTP_FORMAT));
		response().set("Cache-Control",
			"max-age=" + to_string(shift.totalSeconds()) + ", must-revalidate");
	}
}

void PocoRestRequestHandler::run()
{
	ThreadNamer namer("restui-" + request().clientAddress().toString());

	doHandleRequest();

	if (logger().information()) {
		logger().information("duration: "
				+ to_string(m_started.elapsed()) + "us",
				__FILE__, __LINE__);
	}
}

void PocoRestRequestHandler::setupLanguage(RestFlow &flow)
{
	const string language = request().has("Accept-Language") ? request().get("Accept-Language") : "";
	const Locale &httpLocale = m_localeExtractor.extract(language);
	flow.setLocale(httpLocale);

	if (logger().debug())
		logger().debug("resolved HTTP locale: " + httpLocale.toString());

	const Locale &locale = flow.locale(request().get("X-Prefer-Request-Language", "no") == "yes");

	if (logger().debug())
		logger().debug("using locale: " + locale.toString());

	Translator::Ptr translator = m_translatorFactory.create(locale);
	flow.setTranslator(translator);
}

void PocoRestRequestHandler::doHandleRequest()
{
	if (logger().debug()) {
		logger().debug("serving request "
			+ request().getMethod()
			+ " "
			+ request().getURI()
			+ " via action "
			+ m_action->fullName()
			+ " "
			+ asString(m_params),
			__FILE__, __LINE__);
	}

	if (logger().trace()) {
		for (const auto &pair : request()) {
			logger().trace(
				pair.first
				+ ": "
				+ pair.second,
				__FILE__, __LINE__);
		}
	}

	if (!expectedContentLength()) {
		response().send();
		return;
	}

	Poco::URI uri(request().getURI());

	RestFlow flow(
		m_linker,
		uri,
		m_params,
		PocoRequest(request()),
		PocoResponse(response())
	);

	MappedRestAction::Ptr mapped = m_action.cast<MappedRestAction>();
	if (mapped.isNull())
		prepareInternalAction(m_action);
	else
		prepareMappedAction(mapped);

	const auto &call = m_action->call();

	try {
		m_filterChain.applyChain(request(), response());
		if (!response().sent()) {
			flow.setSession(m_session);
			setupLanguage(flow);
			call(flow);
		}

		if (logger().information()) {
			logger().information("result of "
				+ request().getMethod()
				+ " "
				+ request().getURI()
				+ ": "
				+ to_string(response().getStatus()),
				__FILE__, __LINE__);
		}

		if (logger().trace()) {
			for (const auto &pair : response()) {
				logger().trace(
					pair.first
					+ ": "
					+ pair.second,
					__FILE__, __LINE__);
			}
		}

		if (!response().sent())
			response().send();

		return;
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}
	catch (const exception &e) {
		logger().critical(e.what(), __FILE__, __LINE__);
	}
	catch (const char *m) {
		logger().fatal(m, __FILE__, __LINE__);
	}
	catch (...) {
		logger().fatal("unknown error occured", __FILE__, __LINE__);
	}

	if (response().sent())
		return;

	response().setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
}

PocoRestRequestFactory::PocoRestRequestFactory(
		RestRouter &router,
		SessionVerifier &verifier,
		TranslatorFactory &factory,
		HTTPLocaleExtractor &localeExtractor,
		HTTPFilterChain::Ptr filterChain):
	m_router(router),
	m_sessionVerifier(verifier),
	m_translatorFactory(factory),
	m_localeExtractor(localeExtractor),
	m_filterChain(filterChain)
{
}

PocoRestRequestHandler *PocoRestRequestFactory::createHandler(
		RestAction::Ptr action)
{
	PocoRestRequestHandler *handler = nullptr;

	try {
		handler = new PocoRestRequestHandler(
			static_cast<RestLinker &>(m_router),
			m_translatorFactory,
			m_localeExtractor,
			*m_filterChain
		);

		handler->setAction(action);

		return handler;
	}
	catch (...) {
		if (handler)
			delete handler;
		throw;
	}
}

PocoRestRequestHandler *PocoRestRequestFactory::routeBuiltin(const string &name)
{
	RestAction::Ptr target = m_router.lookup("builtin", name);
	if (target.isNull())
		throw Exception("missing handler builtin." + name);

	return createHandler(target);
}

PocoRestRequestHandler *PocoRestRequestFactory::handleNoRoute(const HTTPServerRequest &request)
{
	if (logger().debug()) {
		logger().debug("no action resolved for "
			+ request.getMethod()
			+ " "
			+ request.getURI(),
			__FILE__, __LINE__);
	}

	return routeBuiltin("noroute");
}

PocoRestRequestHandler *PocoRestRequestFactory::handleNoSession()
{
	if (logger().debug()) {
		logger().debug("missing session, redirecting...",
			__FILE__, __LINE__);
	}

	return routeBuiltin("unauthorized");
}

PocoRestRequestHandler *PocoRestRequestFactory::createWithSession(
		RestAction::Ptr action,
		const MappedRestAction::Params &params,
		const HTTPServerRequest &request)
{
	Session::Ptr session;

	if (action->sessionRequired()) {
		try {
			session = m_sessionVerifier.verifyAuthorized(request);
		} catch (const NotAuthenticatedException &e) {
			logger().log(e, __FILE__, __LINE__);
		}

		if (session.isNull())
			return handleNoSession();
	}

	PocoRestRequestHandler *handler = nullptr;
	try {
		handler = createHandler(action);
		handler->setActionParams(params);
		handler->setSession(session);
		return handler;
	}
	catch (...) {
		if (handler)
			delete handler;
		throw;
	}
}

HTTPRequestHandler *PocoRestRequestFactory::createRequestHandler(
		const HTTPServerRequest &request)
{
	const Clock started;

	if (logger().information()) {
		logger().information("handling request "
			+ request.getMethod()
			+ " "
			+ request.getURI()
			+ " from "
			+ request.clientAddress().toString(),
			__FILE__, __LINE__);
	}

	Poco::URI uri(Sanitize::uri(request.getURI()));
	MappedRestAction::Params params;

	try {
		RestAction::Ptr action = m_router.route(
			request.getMethod(), uri, params
		);

		if (action.isNull())
			return handleNoRoute(request);

		PocoRestRequestHandler *handler = createWithSession(
			action,
			params,
			request
		);

		handler->setStarted(started);

		return handler;
	} catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		e.rethrow();
	} catch (const exception &e) {
		logger().fatal(e.what(), __FILE__, __LINE__);
		throw e;
	} catch (...) {
		logger().fatal("something is terribly wrong",
				__FILE__, __LINE__);
		throw;
	}

	logger().fatal("should ben ever reached", __FILE__, __LINE__);
	return NULL;
}

