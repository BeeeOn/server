#include "ControlNotification.h"

using namespace std;



ControlNotification::ControlNotification(string name, int userId,
    int notificationId, vector<string> gcmIds, long time)
: BaseGcmNotification(name, userId, notificationId, gcmIds, time),
  TYPE_CONTROL("control"), LEVEL_CONTROL(100){

}

int ControlNotification::getLevel() {
    return LEVEL_CONTROL;
}

string ControlNotification::getType() {
    return TYPE_CONTROL;
}
