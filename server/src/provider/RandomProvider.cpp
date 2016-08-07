#include "provider/RandomProvider.h"
#include "provider/PocoRandomProvider.h"
#include "provider/MockRandomProvider.h"

BEEEON_OBJECT(PocoRandomProvider, BeeeOn::PocoRandomProvider)
BEEEON_OBJECT(MockRandomProvider, BeeeOn::MockRandomProvider)
BEEEON_OBJECT(InsecureRandomProvider, BeeeOn::InsecureRandomProvider)
