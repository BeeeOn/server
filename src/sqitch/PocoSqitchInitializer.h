#pragma once

#include "dao/poco/PocoDBInitializer.h"
#include "sqitch/SqitchInitializer.h"

namespace BeeeOn {

class PocoSqitchInitializer : public PocoDBInitializer, public SqitchInitializer {
public:
	void initialize(Poco::Data::Session &session) override;
};

}
