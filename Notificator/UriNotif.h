/* 
 * File:   UriNotif.h
 * Author: Martin
 *
 */

#ifndef URINOTIFICATION_H
#define	URINOTIFICATION_H

#include "AdvertNotification.h"
#include <string>
#include "Utils.h"

class UriNotif : public AdvertNotification {
public:
    UriNotif(int userId, int notificationId,
            long time, string message, string uri);
    ~UriNotif();
    string getDbXml();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
private:
    string mMsg, mUri;

};

#endif	/* URINOTIFICATION_H */

