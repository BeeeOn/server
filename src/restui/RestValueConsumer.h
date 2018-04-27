#ifndef BEEEON_REST_VALUE_CONSUMER_H
#define BEEEON_REST_VALUE_CONSUMER_H

#include "service/ValueConsumer.h"
#include "util/Loggable.h"

namespace Poco {
namespace JSON {

class PrintHandler;

}
}

namespace BeeeOn {

namespace RestUI {

class RestValueConsumer : public ValueConsumer, protected Loggable {
public:
	RestValueConsumer(Poco::JSON::PrintHandler &output);

	bool hasBegin() const;

	void begin(const TypeInfo &info) override;
	void single(const ValueAt &v) override;
	void frequency(const ValueAt &v, size_t count) override;
	void end() override;

	static void format(
		Poco::JSON::PrintHandler &output,
		const ValueAt &v,
		const TypeInfo &info);

	static void format(
		Poco::JSON::PrintHandler &output,
		const ValueAt &v,
		const size_t frequency,
		const TypeInfo &info);

protected:
	const TypeInfo &info() const;

private:
	Poco::JSON::PrintHandler &m_output;
	const TypeInfo *m_info;
	size_t m_count;
	bool m_hasBegin;
};

}
}

#endif
