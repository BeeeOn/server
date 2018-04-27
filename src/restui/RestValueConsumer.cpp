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
	m_count(0),
	m_hasBegin(false)
{
}

bool RestValueConsumer::hasBegin() const
{
	return m_hasBegin;
}

void RestValueConsumer::begin(const TypeInfo &info)
{
	m_info = &info;
	m_count = 0;

	m_output.startObject();
	m_output.key("code");
	m_output.value(200);
	m_output.key("status");
	m_output.value(string("success"));
	m_output.key("data");
	m_output.startArray();

	m_hasBegin = true;
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

void RestValueConsumer::frequency(const ValueAt &v, size_t count)
{
	format(m_output, v, count, info());
	m_count += 1;
}

void RestValueConsumer::end()
{
	m_output.endArray();
	m_output.endObject();

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

void RestValueConsumer::format(
		PrintHandler &output,
		const ValueAt &v,
		const size_t frequency,
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

	output.key("frequency");
	output.value(frequency);

	output.endObject();
}
