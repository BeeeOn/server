#ifndef CONTROLNOTIFICATION_H
#define CONTROLNOTIFICATION_H

#include "BaseGcmNotification.h"

using namespace std;

#include <string>

class ControlNotification : public BaseGcmNotification
{
    public:
        virtual string getJson() = 0;
        virtual ~ControlNotification() {};
    protected:
        ControlNotification(string email, int notificationId, 
                vector<string> gcmIds, long time, string action);
        string getType();
    private:
        const string TYPE_CONTROL;
};

#endif // CONTROLNOTIFICATION_H
