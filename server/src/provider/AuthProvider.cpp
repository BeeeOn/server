#include "di/Injectable.h"
#include "provider/AuthProvider.h"
#include "provider/PermitAuthProvider.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PermitAuthProvider)
BEEEON_OBJECT_CASTABLE(AuthProvider)
BEEEON_OBJECT_CASTABLE(AuthCodeAuthProvider)
BEEEON_OBJECT_TEXT("resultProvider", &PermitAuthProvider::setResultProvider)
BEEEON_OBJECT_END(BeeeOn, PermitAuthProvider)
