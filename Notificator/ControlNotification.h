#ifndef CONTROLNOTIFICATION_H
#define CONTROLNOTIFICATION_H

#include "BaseGcmNotification.h"

using namespace std;

#include <string>

class ControlNotification : public BaseGcmNotification
{
    public:
        virtual string getDbXml() = 0;
        virtual string getJson() = 0;
        virtual ~ControlNotification() {};
        
        string getType();
        int getLevel();

    protected:
        ControlNotification (string name, int userId, int notificationId, 
                vector<string> gcmIds, long time);
    
    private:
        const string TYPE_CONTROL;
        const int LEVEL_CONTROL;

};

#endif // CONTROLNOTIFICATION_H
