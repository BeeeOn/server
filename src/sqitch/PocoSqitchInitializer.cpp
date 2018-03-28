#include "di/Injectable.h"
#include "sqitch/PocoSqitchInitializer.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSqitchInitializer)
BEEEON_OBJECT_CASTABLE(PocoDBInitializer)
BEEEON_OBJECT_CASTABLE(SqitchInitializer)
BEEEON_OBJECT_PROPERTY("planFile", &PocoSqitchInitializer::loadPlanFile)
BEEEON_OBJECT_PROPERTY("baseDir", &PocoSqitchInitializer::setBaseDir)
BEEEON_OBJECT_PROPERTY("extension", &PocoSqitchInitializer::setExtension)
BEEEON_OBJECT_END(BeeeOn, PocoSqitchInitializer)

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

void PocoSqitchInitializer::initialize(Poco::Data::Session &session)
{
	unsafeDeploy([&](const string &script) {
		return execute(session, script);
	});
}
