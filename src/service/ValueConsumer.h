#ifndef BEEEON_VALUE_CONSUMER_H
#define BEEEON_VALUE_CONSUMER_H

#include <vector>

#include "model/TypeInfo.h"
#include "model/ValueAt.h"

namespace BeeeOn {

class ValueConsumer {
public:
	virtual void begin(const TypeInfo &info) = 0;
	virtual void single(const ValueAt &v) = 0;
	virtual void multiple(const std::vector<ValueAt> &v) = 0;
	virtual void frequency(const ValueAt &v, size_t count) = 0;
	virtual void end() = 0;
};

}

#endif
