/* 
 * File:   LimitExceededNotification.h
 * Author: Martin
 *
 */

#ifndef LIMITEXCEEDEDNOTIFICATION_H
#define	LIMITEXCEEDEDNOTIFICATION_H

#include "InfoNotification.h"
#include <string>

class LimitExceededNotification : public InfoNotification {
public:
    LimitExceededNotification(string email, int notificationId,
            vector<string> gcmIds, long time, string message);
    ~LimitExceededNotification();
    string getJson();
private:

};

#endif	/* LIMITEXCEEDEDNOTIFICATION_H */

