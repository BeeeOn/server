#include "di/Injectable.h"
#include "dao/UserDao.h"

BEEEON_OBJECT_BEGIN(BeeeOn, MockUserDao)
BEEEON_OBJECT_CASTABLE(UserDao)
BEEEON_OBJECT_END(BeeeOn, MockUserDao)

using namespace BeeeOn;

UserDao::~UserDao()
{
}
