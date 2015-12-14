#ifndef JSONNOTIFICATIONBUILDER_H
#define JSONNOTIFICATIONBUILDER_H

#include <string>
#include <vector>

using namespace std;

class JsonNotificationBuilder
{
    public:
        JsonNotificationBuilder();
        JsonNotificationBuilder addData(string key, string value);
        JsonNotificationBuilder collapseKey(string value);
        JsonNotificationBuilder delayWhileIdle(bool value);
        JsonNotificationBuilder restrictedPackageName(string value);
        JsonNotificationBuilder timeToLive(int value);
        JsonNotificationBuilder dryRun(bool value);
        JsonNotificationBuilder notificationKey(string value);
        JsonNotificationBuilder registrationIds(vector<string> *ids);
        string build();
        ~JsonNotificationBuilder();
    protected:
    private:
        static string vectorToJsonArray(vector<string> *array);

        string mCollapseKey, mRestrictedPackageName, mNotificationKey;
        int mTimeToLive;
        bool mDryRun, mDelayWhileIdle,isCollapse, isRestricted, isDelay, isNotificationKey ,isTTL , isDryRun, isRegIds;
        vector <string> mKeys;
        vector <string> mValues;
        vector <string> *mIds;
};

#endif // JSONNOTIFICATIONBUILDER_H
