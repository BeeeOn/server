#include <iostream>
#include <string>
#include "WatchdogNotif.h"
#include <vector>

using namespace std;

int main()
{   
    vector <string> ids;
    ids.push_back("APA91bH32oxlBDYis-iOnmnQSlPQ65AwjoNIwlVjhZdvZNfFI7exUubTF1SK4M0qukXWPt-9VmbYneGu6098KSqgVtc14Pnukk2sf8Hu5UbRx__vOS6wCmwYG6JD_J9NzQp65WqspOu9aqG_f1O4mkkQqTJnb7P0ww");
    Notification *notif = new WatchdogNotif(
        7,                  // user ID
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

    delete(notif);
    return 0;
}
