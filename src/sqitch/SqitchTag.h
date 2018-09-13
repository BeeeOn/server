#pragma once

#include <string>

#include "sqitch/SqitchEntry.h"

namespace BeeeOn {

class SqitchTag : public SqitchEntry {
public:
	typedef Poco::SharedPtr<SqitchTag> Ptr;

	SqitchTag(unsigned int line);

	std::string toString() const override;
};

}
