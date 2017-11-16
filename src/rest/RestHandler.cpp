#include <Poco/Logger.h>

#include "rest/RestHandler.h"

using namespace std;
using namespace BeeeOn;

RestHandler::RestHandler(const string &name):
	m_name(name)
{
}

RestHandler::~RestHandler()
{
}

RestHandler::Handler RestHandler::wrapHandler(const Handler &handler)
{
	return handler;
}

void RestHandler::registerAction(
		const string &name,
		Handler handler,
		const vector<string> &params)
{
	if (logger().debug()) {
		logger().debug("registering action " + name,
				__FILE__, __LINE__);
	}

	m_actions.push_back(
		{
			name,
			wrapHandler(handler),
			params
		}
	);
}

const vector<RestHandler::Action> &RestHandler::actions() const
{
	return m_actions;
}

const std::string &RestHandler::name() const
{
	return m_name;
}
