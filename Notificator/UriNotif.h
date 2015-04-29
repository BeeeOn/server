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
#include <sstream>

class UriNotif : public AdvertNotification {
public:
    UriNotif(int userId, int notificationId,
            long time, string message, string uri);
    ~UriNotif();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
    void addDbXmlData(stringstream *ss);

private:
    string mMsg, mUri;

};

#endif	/* URINOTIFICATION_H */

