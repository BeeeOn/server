/* 
 * File:   VissibleNotification.h
 * Author: Martin
 *
 * Created on 1. prosinec 2014, 23:22
 */

#ifndef VISIBLENOTIFICATION_H
#define	VISIBLENOTIFICATION_H

#include <string>
#include "BaseNotification.h"

using namespace std;

class VisibleNotification : public BaseNotification
{
public:
    virtual string getDbXml() = 0;
    virtual int getLevel() = 0;
    virtual ~VisibleNotification();
    
    string getId();
    string getTime();
    string getUserId();

protected:
    VisibleNotification(string name, int userId, int notificationId, 
                long time, string message);
    virtual string getType() = 0;
    virtual string getGcmMsg(string ids) = 0;
    string getMessage();

private:
    string mMessage;
};

#endif	/* VISSIBLENOTIFICATION_H */

