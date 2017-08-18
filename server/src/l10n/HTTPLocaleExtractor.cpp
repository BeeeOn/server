#include <set>
#include <vector>

#include <Poco/Logger.h>
#include <Poco/NumberParser.h>
#include <Poco/Net/MessageHeader.h>
#include <Poco/Net/NameValueCollection.h>

#include "l10n/HTTPLocaleExtractor.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

HTTPLocaleExtractor::HTTPLocaleExtractor()
{
}

void HTTPLocaleExtractor::setLocaleManager(SharedPtr<LocaleManager> manager)
{
	m_localeManager = manager;
}

Locale HTTPLocaleExtractor::extract(const string &header)
{
	vector<string> elements;
	MessageHeader::splitElements(header, elements);

	if (logger().trace())
		logger().trace("extracting locale from " + header, __FILE__, __LINE__);

	set<HTTPLocale> sorted;

	for (const auto &element : elements) {
		NameValueCollection parameters;
		string value;

		MessageHeader::splitParameters(element, value, parameters);

		HTTPLocale locale;
		locale.setUnresolvedLocale(value);

		if (logger().trace())
			logger().trace("locale: " + value, __FILE__, __LINE__);

		for (const auto &param : parameters) {
			if (param.first != "q")
				continue;

			const int weight = parseWeight(param.second);
			if (weight < 0)
				continue;

			if (logger().trace()) {
				logger().trace("weight: " + to_string(weight),
						__FILE__, __LINE__);
			}

			locale.setWeight(weight);
			break;
		}

		sorted.insert(locale);
	}

	if (sorted.empty())
		return m_localeManager->parse("");

	vector<string> unresolvedLocales;

	for (const auto &current : sorted)
		unresolvedLocales.push_back(current.unresolvedLocale());

	return m_localeManager->chooseBest(unresolvedLocales);
}

int HTTPLocaleExtractor::parseWeight(const string &input)
{
	try {
		const double weight = NumberParser::parseFloat(input);
		if (weight < 0)
			return -1;

		if (weight > 1)
			return -1;

		return (int) (weight * 1000);
	}
	catch (const SyntaxException &e) {
		return -1;
	}
}

HTTPLocaleExtractor::HTTPLocale::HTTPLocale():
	m_weight(1000)
{
}

void HTTPLocaleExtractor::HTTPLocale::setUnresolvedLocale(const string &locale)
{
	m_unresolvedLocale = locale;
}

const string &HTTPLocaleExtractor::HTTPLocale::unresolvedLocale() const
{
	return m_unresolvedLocale;
}

void HTTPLocaleExtractor::HTTPLocale::setWeight(const unsigned int weight)
{
	m_weight = weight;
}

unsigned int HTTPLocaleExtractor::HTTPLocale::weight() const
{
	return m_weight;
}

bool HTTPLocaleExtractor::HTTPLocale::betterWeight(const HTTPLocale &other) const
{
	return m_weight > other.m_weight;
}
