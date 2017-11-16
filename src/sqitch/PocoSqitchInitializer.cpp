#include "di/Injectable.h"
#include "sqitch/PocoSqitchInitializer.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSqitchInitializer)
BEEEON_OBJECT_CASTABLE(PocoDBInitializer)
BEEEON_OBJECT_CASTABLE(SqitchInitializer)
BEEEON_OBJECT_TEXT("planFile", &PocoSqitchInitializer::loadPlanFile)
BEEEON_OBJECT_TEXT("baseDir", &PocoSqitchInitializer::setBaseDir)
BEEEON_OBJECT_TEXT("extension", &PocoSqitchInitializer::setExtension)
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
