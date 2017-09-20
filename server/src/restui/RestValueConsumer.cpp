#include <cmath>
#include <string>

#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "restui/RestValueConsumer.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

RestValueConsumer::RestValueConsumer(PrintHandler &output):
	m_output(output),
	m_info(NULL)
{
}

void RestValueConsumer::begin(const TypeInfo &info)
{
	m_info = &info;
}

const TypeInfo &RestValueConsumer::info() const
{
	if (m_info == NULL)
		throw IllegalStateException("info is missing, forgot to call begin()?");

	return *m_info;
}

void RestValueConsumer::single(const ValueAt &v)
{
	m_output.startObject();

	m_output.key("at");
	m_output.value(v.atRaw());
	m_output.key("value");

	if (std::isnan(v.value()))
		m_output.null();
	else
		m_output.value(info().asString(v.value()));

	m_output.endObject();
}

void RestValueConsumer::end()
{
	m_info = NULL;
}
