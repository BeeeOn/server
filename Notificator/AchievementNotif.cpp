/* 
 * File:   AchievementNotif.cpp
 * Author: Martin
 * 
 */

#include "AchievementNotif.h"
#include "JsonNotificationBuilder.h"
#include "Constants.h"    
#include <iostream>
#include "Utils.h"
#include "XmlHelper.h"

AchievementNotif::AchievementNotif(int userId, int notificationId,
            long time, int achievement)
:AdvertNotification(NAME_ACHIEVEMENT, userId, notificationId, time), 
 mAchievement(achievement)
{
}

void AchievementNotif::addGcmData(JsonNotificationBuilder *builder) {
  builder->addData(JSON_DATA_ACHIEVEMENT, Utils::intToString(mAchievement));
}

void AchievementNotif::addDbXmlData(stringstream *ss) {
  XmlHelper::tagWithValue(ss, DATA_ACHIEVEMENT, Utils::intToString(mAchievement));
}

AchievementNotif::~AchievementNotif() {
}

