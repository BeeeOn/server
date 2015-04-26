#ifndef BASENOTIFICATION_H
#define BASENOTIFICATION_H

#include "Notification.h"
#include <string>
#include <vector>
#include "JsonNotificationBuilder.h"
#include "Constants.h"

using namespace std;

class BaseNotification : public Notification
{
    public:
        virtual int getLevel() = 0;
        virtual string getDbXml() = 0;
        virtual ~BaseNotification() {};
        
        vector<string> sendGcm(vector<string> *ids);       
        string getId();
        string getUserId();
        string getTime();

    protected:
        BaseNotification(string name, int userId, 
            int notificationId, long time);
        string getName();
        string getGcmMsg(string ids);
        virtual void addGcmData(JsonNotificationBuilder *builder) = 0;
        virtual string getType() = 0;
    
    private:
        string mName;
        int mUserId;
        int mNotificationId;
        long mTime;
        
        string getGcmIds(vector<string> *ids);
};

#endif // BASENOTIFICATION_H
