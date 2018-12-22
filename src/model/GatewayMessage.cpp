#include <map>
#include <vector>

#include <Poco/Exception.h>

#include "model/GatewayMessage.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

GatewayMessage::GatewayMessage():
	m_severity(SEVERITY_ERROR)
{
}

GatewayMessage::GatewayMessage(const ID &id):
	Entity<GatewayMessageID>(id),
	m_severity(SEVERITY_ERROR)
{
}

void GatewayMessage::setGateway(const Gateway &gateway)
{
	m_gateway = gateway;
}

const Gateway &GatewayMessage::gateway() const
{
	return m_gateway;
}

void GatewayMessage::setAt(const Timestamp &at)
{
	m_at = at;
}

Timestamp GatewayMessage::at() const
{
	return m_at;
}

void GatewayMessage::setSeverity(const Severity &severity)
{
	switch (severity) {
	case SEVERITY_INFO:
	case SEVERITY_WARN:
	case SEVERITY_ERROR:
		m_severity = severity;
		return;
	}

	throw InvalidArgumentException(
		"setting invalid severity: " + to_string(severity));
}

GatewayMessage::Severity GatewayMessage::severity() const
{
	return m_severity;
}

void GatewayMessage::setKey(const string &key)
{
	m_key = key;
}

string GatewayMessage::key() const
{
	return m_key;
}

void GatewayMessage::validateContext(const Object::Ptr context) const
{
	for (auto it = begin(*context); it != end(*context); ++it) {
		if (context->isObject(it)) {
			throw InvalidArgumentException(
				"message context must not have nested objects");
		}

		if (context->isArray(it)) {
			Array::Ptr a = context->getArray(it->first);

			for (auto it = begin(*a); it != end(*a); ++it) {
				if (a->isArray(it)) {
					throw InvalidArgumentException(
						"message context must not have nested arrays");
				}
				if (a->isObject(it)) {
					throw InvalidArgumentException(
						"message context must not have nested objects");
				}
			}
		}
	}
}

void GatewayMessage::setContext(const Object::Ptr context)
{
	validateContext(context);
	m_context = context;
}

Object::Ptr GatewayMessage::context() const
{
	return m_context;
}

string GatewayMessage::translate(Translator &translator) const
{
	vector<Dynamic::Var> args;
	map<string, Dynamic::Var> argsMap;
	const Object::Ptr context = this->context();

	if (!context.isNull()) {
		for (const auto &pair : *context)
			argsMap.emplace(pair.first, pair.second);
	}

	for (const auto &pair : argsMap)
		args.emplace_back(pair.second);

	return translator.vformat(m_key, m_key, args);
}

GatewayMessage::Severity GatewayMessage::parseSeverity(const string &input)
{
	if (input == "info")
		return SEVERITY_INFO;
	if (input == "warn")
		return SEVERITY_WARN;
	if (input == "error")
		return SEVERITY_ERROR;

	throw InvalidArgumentException("unknown severity: " + input);
}

string GatewayMessage::severityAsString(const Severity &severity)
{
	switch (severity) {
	case SEVERITY_INFO:
		return "info";
	case SEVERITY_WARN:
		return "warn";
	case SEVERITY_ERROR:
		return "error";
	}

	throw IllegalStateException(
		"invalid severity: " + to_string(severity));
}
