#include "di/Injectable.h"
#include "provider/RandomProvider.h"
#include "provider/PocoRandomProvider.h"
#include "provider/MockRandomProvider.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoRandomProvider)
BEEEON_OBJECT_CASTABLE(RandomProvider)
BEEEON_OBJECT_CASTABLE(SecureRandomProvider)
BEEEON_OBJECT_PROPERTY("reseedPeriod", &PocoRandomProvider::setReseedPeriod)
BEEEON_OBJECT_END(BeeeOn, PocoRandomProvider)

BEEEON_OBJECT_BEGIN(BeeeOn, MockRandomProvider)
BEEEON_OBJECT_CASTABLE(RandomProvider)
BEEEON_OBJECT_PROPERTY("nextRandom", &MockRandomProvider::setNextRandom);
BEEEON_OBJECT_END(BeeeOn, MockRandomProvider)

BEEEON_OBJECT_BEGIN(BeeeOn, InsecureRandomProvider)
BEEEON_OBJECT_CASTABLE(RandomProvider)
BEEEON_OBJECT_CASTABLE(SecureRandomProvider)
BEEEON_OBJECT_PROPERTY("providerImpl", &InsecureRandomProvider::setProviderImpl);
BEEEON_OBJECT_END(BeeeOn, InsecureRandomProvider)
