/* 
 * File:   VissibleNotification.h
 * Author: Martin
 *
 * Created on 1. prosinec 2014, 23:22
 */

#ifndef VISIBLENOTIFICATION_H
#define	VISIBLENOTIFICATION_H

#include <string>
#include "BaseGcmNotification.h"

using namespace std;

class VisibleNotification : public BaseGcmNotification
{
public:
    virtual string getJson() = 0;
    virtual string getDbXml() = 0;
    virtual int getLevel() = 0;
    virtual ~VisibleNotification();
    string getMessage();
    string getUserId();
    string getId();
    string getGcmIds();
    string getTime();
protected:
    VisibleNotification(string name, int userId, int notificationId, 
                vector<string> gcmIds, long time, string message);
    virtual string getType() = 0;

private:
    string mMessage;
};

#endif	/* VISSIBLENOTIFICATION_H */

