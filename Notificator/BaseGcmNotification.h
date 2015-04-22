#ifndef BASENOTIFICATION_H
#define BASENOTIFICATION_H

#include "Notification.h"
#include <string>
#include <vector>

using namespace std;

class BaseGcmNotification : public Notification
{
    public:
        virtual int getLevel() = 0;
        virtual string getJson() = 0;
        virtual string getDbXml() = 0;
        virtual ~BaseGcmNotification() {};
        
        vector<string> send();       
        string getId();
        string getUserId();
        string getGcmIds();
        string getTime();
        string getName();

    protected:
        BaseGcmNotification(string name, int userId, int notificationId, 
                vector<string> gcmIds, long time);
    private:
        string mName;
        int mUserId;
        vector<string> mGcmIds;
        int mNotificationId;
        long mTime;
};

#endif // BASENOTIFICATION_H
