#ifndef CONTROLNOTIFICATION_H
#define CONTROLNOTIFICATION_H

#include "BaseNotification.h"

using namespace std;

#include <string>
#include <sstream>

class ControlNotification : public BaseNotification
{
    public:
        virtual ~ControlNotification() {};
        bool saveToDb();
        int getLevel();

    protected:
        ControlNotification (string name, int userId, int notificationId, 
               long time);
        virtual void addGcmData(JsonNotificationBuilder *builder) = 0;
        virtual void addDbXmlData(stringstream *ss) = 0;
        string getType();
    private:

};

#endif // CONTROLNOTIFICATION_H
