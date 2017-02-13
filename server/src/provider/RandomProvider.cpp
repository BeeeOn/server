#include "provider/RandomProvider.h"
#include "provider/PocoRandomProvider.h"
#include "provider/MockRandomProvider.h"

BEEEON_OBJECT(BeeeOn_PocoRandomProvider, BeeeOn::PocoRandomProvider)
BEEEON_OBJECT(BeeeOn_MockRandomProvider, BeeeOn::MockRandomProvider)
BEEEON_OBJECT(BeeeOn_InsecureRandomProvider, BeeeOn::InsecureRandomProvider)
