#include <string>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/JSON/PrintHandler.h>

#include "restui/RestValueConsumer.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

RestValueConsumer::RestValueConsumer(PrintHandler &output):
	m_output(output),
	m_info(NULL),
	m_count(0)
{
}

void RestValueConsumer::begin(const TypeInfo &info)
{
	m_info = &info;
	m_count = 0;
}

const TypeInfo &RestValueConsumer::info() const
{
	if (m_info == NULL)
		throw IllegalStateException("info is missing, forgot to call begin()?");

	return *m_info;
}

void RestValueConsumer::single(const ValueAt &v)
{
	format(m_output, v, info());
	m_count += 1;
}

void RestValueConsumer::end()
{
	m_info = NULL;

	if (logger().debug()) {
		logger().debug("consumed " + to_string(m_count) + " values",
				__FILE__, __LINE__);
	}

	m_count = 0;
}

void RestValueConsumer::format(
		PrintHandler &output,
		const ValueAt &v,
		const TypeInfo &info)
{
	output.startObject();

	output.key("at");
	output.value(v.atRaw());
	output.key("value");

	if (!v.isValid())
		output.null();
	else
		output.value(info.asString(v.value()));

	output.endObject();
}
