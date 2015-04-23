#include <iostream>
#include <string>
#include "WatchdogNotif.h"
#include <vector>

using namespace std;

int main()
{   
    vector <string> ids;
    ids.push_back("APA91bFq51HkPkbr6t88BPb5Uq7IX-BrOMoZIPP8Y5W_vHd_OscaE84KdH_N-yCLWb8E1bD9sGh3bBvUwORTJxtEY01R6v_5mzY5RQfX60CyhLjJtor3AkET13J6tFH0cKtQejaC7BrG6Zan75KER-lkrsZ56xBCzw");
    Notification *notif = new WatchdogNotif(
        123,                  // user ID
        123,                  // message (notification) ID
        1418220573673,        // timestamp (ms)
        "Jsem nova zprava",   // message
        123,                  // adadpter ID
        "deviceId",           // sensor ID
        1);                   // sensor type
    
    notif->sendGcm(&ids);

    delete(notif);
    return 0;
}
