#include <Poco/Exception.h>
#include <Poco/NumberParser.h>
#include <Poco/String.h>

#include "rest/RestAction.h"
#include "rest/RestFlow.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

RestAction::RestAction(
		const std::string &handlerName,
		const std::string &actionName):
	m_handlerName(handlerName),
	m_actionName(actionName)
{
}

RestAction::~RestAction()
{
}

void RestAction::setCall(const Call &call)
{
	m_call = call;
}

const RestAction::Call &RestAction::call() const
{
	return m_call;
}

void RestAction::setSessionRequired(bool required)
{
	m_sessionRequired = required;
}

bool RestAction::sessionRequired() const
{
	return m_sessionRequired;
}

void RestAction::setInputMaxSize(int bytes)
{
	m_inputMaxSize = bytes < 0? -1 : bytes;
}

int RestAction::inputMaxSize() const
{
	return m_inputMaxSize;
}

void RestAction::execute(RestFlow &flow)
{
	m_call(flow);
}

bool RestAction::parseSessionRequired(const std::string &required)
{
	if (!icompare(required, "yes"))
		return true;
	if (!icompare(required, "no"))
		return false;

	return NumberParser::parseBool(required);
}
