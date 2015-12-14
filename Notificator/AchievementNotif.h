/* 
 * File:   AchievementNotif.h
 * Author: Martin
 *
 */

#ifndef ACHIEVEMENTNOTIFICATION_H
#define	ACHIEVEMENTNOTIFICATION_H

#include "AdvertNotification.h"
#include <string>
#include "Utils.h"
#include <sstream>

class AchievementNotif : public AdvertNotification {
public:
    AchievementNotif(int userId, int notificationId,
            long time, int achievement);
    ~AchievementNotif();
protected:
    void addGcmData(JsonNotificationBuilder *builder);
    void addDbXmlData(stringstream *ss);

private:
    int mAchievement;

};

#endif	/* ACHIEVEMENTNOTIFICATION_H */

