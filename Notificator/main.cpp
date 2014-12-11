#include <iostream>
#include <string>
#include "Notificator.h"
#include "LimitExceededNotification.h"
#include <vector>

using namespace std;

int main()
{   
    vector <string> ids;
    ids.push_back("APA91bHGqKoRnpLTUeHnLNIVLyOkXKqWiKxJn0CzCED55jMpDLxq85wBalb_p06mX27nwo9vx6ahKmhJAMYyqHVWPe1_u50ons-zatXX8S3vV2vtkH4BUW1J-qgvjDXxziDAwu09qreCJu8PWhHKOOSYSsaYYxeZxQ");
    Notification *notif = new LimitExceededNotification("doudera.m@gmail.com", 123,
            ids, 1418220573673, "Jsem nova zprava");
    Notificator::sendNotification(*notif);
    
    delete(notif);
    return 0;
}
