#ifndef BASENOTIFICATION_H
#define BASENOTIFICATION_H

#include "Notification.h"
#include <string>
#include <vector>

using namespace std;

class BaseGcmNotification : public Notification
{
    public:
        virtual string getJson() = 0;
        virtual ~BaseGcmNotification() {};
        string getEmail();
        string getNotificationId();
        string getGcmIds();
        string getTime();
        string getAction();
    protected:
        BaseGcmNotification(string email, int notificationId, 
                vector<string> gcmIds, long time, string action);
        virtual string getType() = 0;
    private:
        string mEmail;
        vector<string> mGcmIds;
        int mNotificationId;
        long mTime;
        string mAction;
};

#endif // BASENOTIFICATION_H
