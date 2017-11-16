#include <vector>

#include <Poco/NumberParser.h>
#include <Poco/String.h>

#include "rest/MappedRestAction.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

MappedRestAction::MappedRestAction(
		const std::string &handlerName,
		const std::string &actionName,
		const UriPattern &pattern):
	RestAction(handlerName, actionName),
	m_uriPattern(pattern)
{
}

void MappedRestAction::setCaching(int seconds)
{
	m_caching = seconds;
}

int MappedRestAction::caching() const
{
	return m_caching;
}

URI MappedRestAction::uri(const vector<string> &params) const
{
	return m_uriPattern.format(params);
}

bool MappedRestAction::match(const Poco::URI &uri, Params &params) const
{
	return m_uriPattern.match(uri, params);
}

int MappedRestAction::parseCaching(const std::string &caching)
{
	if (icompare(caching, "off"))
		return -1;
	if (icompare(caching, "ignore"))
		return -1;
	if (icompare(caching, "no"))
		return 0;
	if (icompare(caching, "never"))
		return 0;

	int value = NumberParser::parse(caching);
	if (value < 0)
		return -1;

	return value;
}
