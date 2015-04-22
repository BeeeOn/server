/* 
 * File:   InfoNotification.h
 * Author: Martin
 *
 * Created on 2. prosinec 2014, 0:13
 */

#ifndef INFONOTIFICATION_H
#define	INFONOTIFICATION_H

#include <string>
#include <vector>
#include "VisibleNotification.h"

class InfoNotification : public VisibleNotification {
public:
    virtual string getJson() = 0;
    virtual string getDbXml() = 0;
    virtual ~InfoNotification();

    string getMessage();
    string getUserId();
    string getId();
    string getGcmIds();
    string getTime();
    string getType();
    int getLevel();
protected:
    InfoNotification(string name, int userId, int notificationId,
            vector<string> gcmIds, long time, string message);
private:
    const string TYPE_INFO;
    const int LEVEL_INFO;
};

#endif	/* INFONOTIFICATION_H */

