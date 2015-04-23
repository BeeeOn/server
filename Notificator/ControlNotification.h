#ifndef CONTROLNOTIFICATION_H
#define CONTROLNOTIFICATION_H

#include "BaseNotification.h"

using namespace std;

#include <string>

class ControlNotification : public BaseNotification
{
    public:
        virtual string getDbXml() = 0;
        virtual ~ControlNotification() {};
        
        int getLevel();

    protected:
        ControlNotification (string name, int userId, int notificationId, 
               long time);
    
        virtual string getGcmMsg(string ids) = 0;
        string getType();
    private:

};

#endif // CONTROLNOTIFICATION_H
