#ifndef BEEEON_POCO_SQITCH_INITIALIZER_H
#define BEEEON_POCO_SQITCH_INITIALIZER_H

#include "dao/poco/PocoDBInitializer.h"
#include "sqitch/SqitchInitializer.h"

namespace BeeeOn {

class PocoSqitchInitializer : public PocoDBInitializer, public SqitchInitializer {
public:
	void initialize(Poco::Data::Session &session) override;
};

}

#endif
