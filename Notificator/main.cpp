#include <iostream>
#include <string>
#include "Notificator.h"
#include "LimitExceededNotification.h"
#include "Notificator.h"
#include <vector>

using namespace std;

int main()
{   
    vector <string> ids;
    ids.push_back("APA91bFq51HkPkbr6t88BPb5Uq7IX-BrOMoZIPP8Y5W_vHd_OscaE84KdH_N-yCLWb8E1bD9sGh3bBvUwORTJxtEY01R6v_5mzY5RQfX60CyhLjJtor3AkET13J6tFH0cKtQejaC7BrG6Zan75KER-lkrsZ56xBCzw");
    Notification *notif = new LimitExceededNotification(
        123,
        123,
        ids, 
        1418220573673, 
        "Jsem nova zprava",
        123, 
        "deviceId", 
        1);
    Notificator::sendNotification(*notif);
    
    delete(notif);
    return 0;
}
