/* 
 * File:   AdvertNotification.h
 * Author: Martin
 *
 * Created on 2. prosinec 2014, 0:17
 */

#ifndef ADVERTNOTIFICATION_H
#define	ADVERTNOTIFICATION_H

#include <string>
#include <vector>
#include "VisibleNotification.h"

class AdvertNotification : VisibleNotification{
public:
    virtual string getJson() = 0;
    virtual string getDbXml() = 0;
    virtual ~AdvertNotification();
    
    string getType();
    int getLevel();

protected:
    AdvertNotification(string name, int userId, int notificationId,
            vector<string> gcmIds, long time,  string message);
private:
    const string TYPE_ADVERT;
    const int LEVEL_ADVERT;
};

#endif	/* ADVERTNOTIFICATION_H */

