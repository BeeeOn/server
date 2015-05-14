#include <iostream>
#include <string>
#include "WatchdogNotif.h"
#include "DeleteNotif.h"
#include "AchievementNotif.h"
#include <vector>

using namespace std;

int main()
{   

    vector <string> ids;
    ids.push_back("APA91bF-u3AZ3bNl6jehq19oEhbVFGEaA_4x3jlzMtKwHzlakRcE9K_H_Rk8YuNSjp1K62dz-_sL09iQjlH8Z3JTsxbg1VbV_Gch0YhTuOg3U1bCib7pk4Xg4rHPeUrICiQ74RIB3qXxTjmTCeLBZor_Tr3pLuATUw");
   int i = 0;
    Notification *notif = new WatchdogNotif(
        8,                    // user ID
        i,                  // message (notification) ID
        1431593526000,        // timestamp (ms)
        "Vlhkost v obýváku překročila 60%",   // message
        54321,                  // adadpter ID
        "774312",               // sensor ID
        2,                  // sensor type
        3);                   // rule ID
    
    notif->sendGcm(&ids);
 /*
    sleep(5);   

    Notification *notif2 = new DeleteNotif(
        8,
        2,
        45,
        0);

    notif2->sendGcm(&ids);
   */ 
    if (notif->saveToDb()) {
      cout << endl << "########   SAVING TO DATABASE ############" << endl;
      int userId = notif->getUserId();
      long timestamp = notif->getTime();
      int notificationId = notif->getId();
      int level = notif->getLevel();
      string name = notif->getName();
      cout << notif->getDbXml() << endl;
    }

    /*
    Notification *notif2 = new AchievementNotif(
        8,                  // user ID
        123,                // message (notification) ID
        1418220573673,      // timestamp (ms)
        1                   // achievement ID
        );

    notif2->sendGcm(&ids);
*/
    delete(notif);
  
    return 0;
}
