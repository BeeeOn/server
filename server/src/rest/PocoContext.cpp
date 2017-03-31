#include <iostream>
#include <vector>

#include <Poco/String.h>

#include "rest/PocoContext.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

PocoRequest::PocoRequest(HTTPServerRequest &impl):
	PocoContext<HTTPServerRequest>(impl)
{
}

istream &PocoRequest::stream() const
{
	return m_impl.stream();
}

bool PocoRequest::accepts(const string &type, const string &subtype) const
{
	if (!m_impl.has("Accept"))
		return true;

	const string fulltype(type + "/" + subtype);

	vector<string> elements;
	MessageHeader::splitElements(m_impl["Accept"], elements);

	for (const string &element : elements) {
		string value;
		NameValueCollection params;
		MessageHeader::splitParameters(element, value, params);

		if (icompare(value, fulltype) == 0)
			return true;
		else if(icompare(value, type + "/*") == 0)
			return true;
	}

	return false;
}

PocoResponse::PocoResponse(HTTPServerResponse &impl):
	PocoContext<HTTPServerResponse>(impl)
{
}

PocoHeaderAccessor<HTTPServerResponse> PocoResponse::at(const string &key)
{
	return PocoHeaderAccessor<HTTPServerResponse>(m_impl, key);
}

void PocoResponse::setStatus(unsigned int status)
{
	m_impl.setStatusAndReason((HTTPResponse::HTTPStatus) status);
}

unsigned int PocoResponse::status() const
{
	return m_impl.getStatus();
}

void PocoResponse::sendAll(const void *p, std::size_t len)
{
	m_impl.sendBuffer(p, len);
}

ostream &PocoResponse::stream()
{
	return m_impl.send();
}
