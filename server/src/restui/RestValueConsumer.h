#ifndef BEEEON_REST_VALUE_CONSUMER_H
#define BEEEON_REST_VALUE_CONSUMER_H

#include "service/ValueConsumer.h"

namespace Poco {
namespace JSON {

class PrintHandler;

}
}

namespace BeeeOn {

namespace RestUI {

class RestValueConsumer : public ValueConsumer {
public:
	RestValueConsumer(Poco::JSON::PrintHandler &output);

	void begin(const TypeInfo &info) override;
	void single(const ValueAt &v) override;
	void end() override;

protected:
	const TypeInfo &info() const;

private:
	Poco::JSON::PrintHandler &m_output;
	const TypeInfo *m_info;
};

}
}

#endif
