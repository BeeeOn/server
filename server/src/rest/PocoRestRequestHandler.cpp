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

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

PocoRestRequestHandler::PocoRestRequestHandler(
		RestAction::Ptr action,
		const MappedRestAction::Params &params,
		ExpirableSession::Ptr session,
		RestLinker &linker,
		TranslatorFactory &factory,
		HTTPLocaleExtractor &localeExtractor):
	m_action(action),
	m_params(params),
	m_session(session),
	m_linker(linker),
	m_translatorFactory(factory),
	m_localeExtractor(localeExtractor)
{
}

bool PocoRestRequestHandler::expectedContentLength(
		HTTPServerRequest &req,
		HTTPServerResponse &res)
{
	const string &method = req.getMethod();

	if (method != "POST" && method != "PUT" && method != "PATCH")
		return true;;

	if (!req.hasContentLength()) {
		res.setStatusAndReason(HTTPResponse::HTTP_LENGTH_REQUIRED);
		return false;
	}

	int inputMaxSize = m_action->inputMaxSize();
	if (inputMaxSize < 0)
		return true;

	auto contentLength = req.getContentLength();
	if (contentLength > inputMaxSize) {
		if (logger().warning()) {
			logger().warning(
				"too long input: "
				+ to_string(contentLength)
				+ " for "
				+ m_action->fullName(),
				__FILE__, __LINE__);
		}

		res.setStatusAndReason(HTTPResponse::HTTP_BAD_REQUEST);
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
		RestAction::Ptr action,
		HTTPServerRequest &req,
		HTTPServerResponse &res) const
{
	req.set("Cache-Control", "public, no-cache");
}

void PocoRestRequestHandler::prepareMappedAction(
		MappedRestAction::Ptr action,
		HTTPServerRequest &req,
		HTTPServerResponse &res) const
{
	if (action->caching() == 0) {
		req.set("Cache-Control", "public, no-cache");
	}
	else {
		const Timespan shift(action->caching(), 0);
		const DateTime now;

		res.set("Expires",
			DateTimeFormatter::format(now + shift, DateTimeFormat::HTTP_FORMAT));
		res.set("Cache-Control",
			"max-age=" + to_string(shift.totalSeconds()) + ", must-revalidate");
	}
}

void PocoRestRequestHandler::handleRequest(
		HTTPServerRequest &req,
		HTTPServerResponse &res)
{
	Thread *current = Thread::current();
	if (current != NULL)
		current->setName("restui-" + req.clientAddress().toString());

	doHandleRequest(req, res);

	if (current != NULL)
		current->setName("");
}

void PocoRestRequestHandler::doHandleRequest(
		HTTPServerRequest &req,
		HTTPServerResponse &res)
{
	if (logger().debug()) {
		logger().debug("serving request "
			+ req.getMethod()
			+ " "
			+ req.getURI()
			+ " via action "
			+ m_action->fullName()
			+ " "
			+ asString(m_params),
			__FILE__, __LINE__);
	}

	if (logger().trace()) {
		for (const auto &pair : req) {
			logger().trace(
				pair.first
				+ ": "
				+ pair.second,
				__FILE__, __LINE__);
		}
	}

	if (!expectedContentLength(req, res)) {
		res.send();
		return;
	}

	Poco::URI uri(req.getURI());

	RestFlow flow(
		m_linker,
		uri,
		m_params,
		PocoRequest(req),
		PocoResponse(res)
	);

	MappedRestAction::Ptr mapped = m_action.cast<MappedRestAction>();
	if (mapped.isNull())
		prepareInternalAction(m_action, req, res);
	else
		prepareMappedAction(mapped, req, res);

	const auto &call = m_action->call();

	try {
		flow.setSession(m_session);

		if (m_session.isNull()) {
			string language = req.has("Accept-Language") ? req.get("Accept-Language") : "";

			const Locale &httpLocale = m_localeExtractor.extract(language);

			if (logger().debug())
				logger().debug("resolved locale: " + httpLocale.toString());

			Translator::Ptr translator = m_translatorFactory.create(httpLocale);
			flow.setTranslator(translator);
		}
		else {
			Translator::Ptr translator = m_translatorFactory.create(m_session->locale());

			if (logger().debug())
				logger().debug("using user locale: " + m_session->locale().toString());

			flow.setTranslator(translator);
		}

		call(flow);

		if (logger().information()) {
			logger().information("result of "
				+ req.getMethod()
				+ " "
				+ req.getURI()
				+ ": "
				+ to_string(res.getStatus()),
				__FILE__, __LINE__);
		}

		if (logger().trace()) {
			for (const auto &pair : res) {
				logger().trace(
					pair.first
					+ ": "
					+ pair.second,
					__FILE__, __LINE__);
			}
		}

		if (!res.sent())
			res.send();

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

	if (res.sent())
		return;

	res.setStatusAndReason(HTTPResponse::HTTP_INTERNAL_SERVER_ERROR);
}

PocoRestRequestFactory::PocoRestRequestFactory(
		RestRouter &router,
		SessionVerifier &verifier,
		TranslatorFactory &factory,
		HTTPLocaleExtractor &localeExtractor):
	m_router(router),
	m_sessionVerifier(verifier),
	m_translatorFactory(factory),
	m_localeExtractor(localeExtractor)
{
}

HTTPRequestHandler *PocoRestRequestFactory::handleNoRoute(const HTTPServerRequest &request)
{
	if (logger().debug()) {
		logger().debug("no action resolved for "
			+ request.getMethod()
			+ " "
			+ request.getURI(),
			__FILE__, __LINE__);
	}

	RestAction::Ptr target = m_router.lookup("builtin", "noroute");
	if (target.isNull())
		throw Exception("missing handler builtin.noroute");

	return new PocoRestRequestHandler(
		target,
		{},
		NULL,
		static_cast<RestLinker &>(m_router),
		m_translatorFactory,
		m_localeExtractor
	);
}

HTTPRequestHandler *PocoRestRequestFactory::handleNoSession()
{
	if (logger().debug()) {
		logger().debug("missing session, redirecting...",
			__FILE__, __LINE__);
	}

	RestAction::Ptr target = m_router.lookup("builtin", "unauthorized");
	if (target.isNull())
		throw Exception("missing handler builtin.unauthorized");

	return new PocoRestRequestHandler(
		target,
		{},
		NULL,
		static_cast<RestLinker &>(m_router),
		m_translatorFactory,
		m_localeExtractor
	);
}

HTTPRequestHandler *PocoRestRequestFactory::createWithSession(
		RestAction::Ptr action,
		const MappedRestAction::Params &params,
		const HTTPServerRequest &request)
{
	ExpirableSession::Ptr session;

	if (action->sessionRequired()) {
		try {
			session = m_sessionVerifier.verifyAuthorized(request);
		} catch (const NotAuthenticatedException &e) {
			logger().log(e, __FILE__, __LINE__);
		}

		if (session.isNull())
			return handleNoSession();
	}

	return new PocoRestRequestHandler(
		action,
		params,
		session,
		static_cast<RestLinker &>(m_router),
		m_translatorFactory,
		m_localeExtractor
	);
}

HTTPRequestHandler *PocoRestRequestFactory::createRequestHandler(
		const HTTPServerRequest &request)
{
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

		return createWithSession(
			action,
			params,
			request
		);
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

