#include <iostream>
#include <string>
#include "WatchdogNotif.h"
#include <vector>

using namespace std;

int main()
{   
    vector <string> ids;
    ids.push_back("APA91bFqQjAONaBCNnguzbS6wwvLt2s0V2X8f9o9ZO3VdjjlI8U9C_P-dm6QcXYgsB3b2M8x_EZzO-A6bzMMy3fFjL9lLMRDob3_UKkQ0g8KSdqiwKTdFd7OLU6TrbOfT7KNO28xVHAV0zgtvOKTUMAeuBstzxeHIQ");
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
