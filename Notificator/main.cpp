#include <iostream>
#include <string>
#include "WatchdogNotif.h"
#include "AchievementNotif.h"
#include <vector>

using namespace std;

int main()
{   
    vector <string> ids;
    ids.push_back("APA91bH_8E7ph0hY1cPoSGcznmYzF0pn_cahzm2OrUXPk42Cdr7KS_jxSNQcHLdhxfXEJeS2d_3J_KcHRxDJfcYONRs1jCNVpr4c2GOQP13PFflWIvK7iHAOH_-qD3L7mdz2vwPCyo3q_8_gF0m8KkeJpmHjmo9ETA");
    Notification *notif = new WatchdogNotif(
        8,                  // user ID
        123,                  // message (notification) ID
        1418220573673,        // timestamp (ms)
        "Jsem nova zprava",   // message
        123,                  // adadpter ID
        "deviceId",           // sensor ID
        1,                    // sensor type
        3);                   // rule ID
    
    notif->sendGcm(&ids);
    
    if (notif->saveToDb()) {
      cout << endl << "########   SAVING TO DATABASE ############" << endl;
      int userId = notif->getUserId();
      long timestamp = notif->getTime();
      int notificationId = notif->getId();
      int level = notif->getLevel();
      string name = notif->getName();
      cout << notif->getDbXml() << endl;
    }

    
    Notification *notif2 = new AchievementNotif(
        8,                  // user ID
        123,                // message (notification) ID
        1418220573673,      // timestamp (ms)
        1                   // achievement ID
        );

    notif->sendGcm(&ids);

    delete(notif);
    delete(notif2);

    return 0;
}
