#include "provider/RandomProvider.h"
#include "provider/PocoRandomProvider.h"
#include "provider/MockRandomProvider.h"

BEEEON_OBJECT(BeeeOn, PocoRandomProvider)
BEEEON_OBJECT(BeeeOn, MockRandomProvider)
BEEEON_OBJECT(BeeeOn, InsecureRandomProvider)
