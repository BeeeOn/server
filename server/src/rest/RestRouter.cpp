#include <Poco/Exception.h>
#include <Poco/File.h>
#include <Poco/Logger.h>
#include <Poco/String.h>

#include <Poco/Util/AbstractConfiguration.h>

#include "di/Injectable.h"
#include "rest/MappedRestAction.h"
#include "rest/RestAction.h"
#include "rest/RestHandler.h"
#include "rest/RestRouter.h"
#include "util/ConfigurationLoader.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestRouter)
BEEEON_OBJECT_TEXT("apiName", &RestRouter::setApiName)
BEEEON_OBJECT_TEXT("applicationName", &RestRouter::setApplicationName)
BEEEON_OBJECT_TEXT("configurationFile", &RestRouter::setConfigurationFile)
BEEEON_OBJECT_REF("handlers", &RestRouter::addHandler)
BEEEON_OBJECT_HOOK("done", &RestRouter::initRouting)
BEEEON_OBJECT_END(BeeeOn, RestRouter)

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace BeeeOn;

RestRouter::RestRouter():
	m_applicationName("unknown")
{
}

RestRouter::~RestRouter()
{
}

void RestRouter::setConfigurationFile(const string &path)
{
	File file(path);

	ConfigurationLoader loader;
	loader.load(path);
	loader.finished();

	m_config = loader.config();
}

void RestRouter::setApiName(const std::string &name)
{
	if (name.find(".") != string::npos)
		throw InvalidArgumentException("API name must not contain dot: " + name);

	if (name.empty())
		throw InvalidArgumentException("API name must not not be empty");

	m_name = name;
}

void RestRouter::setApplicationName(const std::string &name)
{
	m_applicationName = name;
}

string RestRouter::applicationName() const
{
	return m_applicationName;
}

void RestRouter::addHandler(RestHandler *handler)
{
	m_handlers.push_back(handler);
}

URI RestRouter::link(
		const string &group,
		const string &name,
		const map<string, string> &queryParams,
		const vector<string> &params)
{
	auto it = m_linkMapper.find(group + "." + name);
	if (it == m_linkMapper.end()) {
		throw IllegalStateException(
			"failed to build link to "
			+ group
			+ "."
			+ name);
	}

	MappedRestAction::Ptr action = it->second.cast<MappedRestAction>();
	if (action.isNull()) {
		throw IllegalStateException(
			"action "
			+ it->second->fullName()
			+ " is not mapped to any URI"
		);
	}

	URI::QueryParameters qpList;

	for (const auto &pair : queryParams)
		qpList.push_back(pair);

	URI uri = action->uri(params);
	uri.setQueryParameters(qpList);
	return uri;
}

void RestRouter::initRouting()
{
	AutoPtr<AbstractConfiguration> view = m_config->createView(m_name + ".api");

	for (auto handler : m_handlers) {
		initHandler(*handler, view);
	}
}

void RestRouter::initHandler(RestHandler &handler,
		AutoPtr<AbstractConfiguration> &config)
{
	if (handler.name().find(".") != string::npos) {
		throw InvalidArgumentException(
			"handler name must not contain a dot: "
			+ handler.name());
	}

	for (auto &action : handler.actions()) {
		if (action.m_name.find(".") != string::npos) {
			throw InvalidArgumentException(
				"handler action must not contain a dot in its name: "
				+ action.m_name);
		}

		initAction(handler, action, config);
	}
}

string RestRouter::assureMethod(const string &method) const
{
	if (!icompare(method, "GET"))
		return "GET";
	if (!icompare(method, "POST"))
		return "POST";
	if (!icompare(method, "PUT"))
		return "PUT";
	if (!icompare(method, "DELETE"))
		return "DELETE";
	if (!icompare(method, "PATCH"))
		return "PATCH";
	if (!icompare(method, "HEAD"))
		return "HEAD";
	if (!icompare(method, "OPTIONS"))
		return "OPTIONS";

	throw InvalidArgumentException("unrecognized HTTP method: " + method);
}

string RestRouter::configMethod(
		AutoPtr<AbstractConfiguration> &config,
		const string &base)
{
	return config->getString(base + ".method",
			config->getString("default.method", "GET"));
}

bool RestRouter::configSessionRequired(
		AutoPtr<AbstractConfiguration> &config,
		const string &base)
{
	const string &required = config->getString(base + ".session.required",
			config->getString("default.session.required", "yes"));

	return RestAction::parseSessionRequired(required);
}

string RestRouter::configCaching(
		AutoPtr<AbstractConfiguration> &config,
		const string &base)
{
	return config->getString(base + ".caching",
			config->getString("default.caching", "no"));
}

int RestRouter::configInputMaxSize(
		AutoPtr<AbstractConfiguration> &config,
		const string &base)
{
	return config->getInt(base + ".input.maxsize",
			config->getInt("default.input.maxsize", 0));
}

RestAction::Ptr RestRouter::createMapped(
		const string &handlerName,
		const RestHandler::Action &stub,
		const string &uri,
		AutoPtr<AbstractConfiguration> &config,
		const string &base)
{
	MappedRestAction::Ptr action = new MappedRestAction(
			handlerName, stub.m_name,
			UriPattern(uri, stub.m_params));

	string method = assureMethod(configMethod(config, base));

	logger().notice("mapping " + action->fullName()
			+ " to " + method + " " + uri,
			__FILE__, __LINE__);

	action->setCaching(
		MappedRestAction::parseCaching(configCaching(config, base)));

	m_methodMapper[method].push_back(action);
	return action;
}

void RestRouter::initAction(RestHandler &handler,
		const RestHandler::Action &stub,
		AutoPtr<AbstractConfiguration> &config)
{
	if (logger().debug()) {
		logger().debug("initializing action "
				+ handler.name() + "." + stub.m_name,
				__FILE__, __LINE__);
	}

	const string base("call." + handler.name() + "." + stub.m_name);

	RestAction::Ptr action;
	const string uri = config->getString(base + ".uri", "");

	if (!uri.empty()) {
		action = createMapped(handler.name(),
				stub, uri, config, base);
	}
	else if (!stub.m_params.empty()) { /* URI not empty, but params defined */
		throw InvalidArgumentException(
			"missing URI for rest action "
			+ handler.name() + "." + stub.m_name
			+ " because it expects URI params"
		);
	}
	else { /* no URI and no params */
		action = new RestAction(handler.name(), stub.m_name);

		logger().notice("registering internal unmapped " + action->fullName(),
				__FILE__, __LINE__);
	}

	action->setCall(stub.m_handler);
	action->setSessionRequired(configSessionRequired(config, base));
	action->setInputMaxSize(configInputMaxSize(config, base));

	m_linkMapper[action->fullName()] = action;
}

MappedRestAction::Ptr RestRouter::route(
		const string &method,
		const URI &uri) const
{
	MappedRestAction::Params params;
	return route(method, uri, params);
}

MappedRestAction::Ptr RestRouter::route(
		const string &method,
		const URI &uri,
		MappedRestAction::Params &params) const
{
	if (logger().debug()) {
		logger().debug("routing " + method + " " + uri.toString(),
				__FILE__, __LINE__);
	}

	auto it = m_methodMapper.find(method);
	if (it == m_methodMapper.end()) {
		if (logger().debug()) {
			logger().debug("no action for method " + method,
					__FILE__, __LINE__);
		}

		return NULL;
	}

	for (auto &action : it->second) {
		params.clear();

		if (action->match(uri, params))
			return action;
	}

	return NULL;
}

RestAction::Ptr RestRouter::lookup(
		const std::string &group,
		const std::string &action) const
{
	auto it = m_linkMapper.find(group + "." + action);
	if (it == m_linkMapper.end())
		return NULL;

	return it->second;
}
