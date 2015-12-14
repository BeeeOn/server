#include "ControlNotification.h"
#include "Constants.h"

using namespace std;



ControlNotification::ControlNotification(string name, int userId,
    int notificationId, long time)
: BaseNotification(name, userId, notificationId, time)
{
}

bool ControlNotification::saveToDb() {
    return false;
}

int ControlNotification::getLevel() {
    return LEVEL_CONTROL;
}

string ControlNotification::getType() {
    return TYPE_CONTROL;
}
