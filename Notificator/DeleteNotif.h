/* 
 * File:   DeleteNotif.h
 * Author: Martin
 *
 */

#ifndef DELETENOTIFICATION_H
#define	DELETENOTIFICATION_H

#include "ControlNotification.h"
#include <string>
#include "Utils.h"

class DeleteNotif : public ControlNotification {
public:
    DeleteNotif(int userId, int notificationId,
            long time, int deleteNotifId);
    ~DeleteNotif();
    string getDbXml();

protected:
    void addGcmData(JsonNotificationBuilder *builder);
private:
    int mDeleteNotifId;
};

#endif	/* DELETENOTIFICATION_H */

