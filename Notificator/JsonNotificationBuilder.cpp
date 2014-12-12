#include <sstream>
#include "JsonNotificationBuilder.h"
#include "JsonUtil.h"
#include "Constants.h"

using namespace std;

JsonNotificationBuilder::JsonNotificationBuilder()
:mCollapseKey(STRING_UNDEFINED), mRestrictedPackageName(STRING_UNDEFINED), mNotificationKey(STRING_UNDEFINED), mRegIds(STRING_UNDEFINED), mTimeToLive(TTL_UNDEFINED),
mDryRun(false), mDelayWhileIdle(false), isCollapse(false), isRestricted(false), isDelay(false), isNotificationKey(false), isTTL(false) , isDryRun(false), isRegIds(false)
{

}

JsonNotificationBuilder JsonNotificationBuilder::addData(string key, string value)
{
    mKeys.push_back(key);
    mValues.push_back(value);
    return *this;
}

JsonNotificationBuilder JsonNotificationBuilder::collapseKey(string value)
{
    isCollapse = true;
    mCollapseKey = value;
    return *this;
}

JsonNotificationBuilder JsonNotificationBuilder::delayWhileIdle(bool value)
{
    isDelay = true;
    mDelayWhileIdle = value;
    return *this;
}
JsonNotificationBuilder JsonNotificationBuilder::restrictedPackageName(string value)
{
    isRestricted = true;
    mRestrictedPackageName = value;
    return *this;
}
JsonNotificationBuilder JsonNotificationBuilder::timeToLive(int value)
{
    isTTL = true;
    mTimeToLive = value;
    return *this;
}
JsonNotificationBuilder JsonNotificationBuilder::dryRun(bool value)
{
    isDryRun = true;
    mDryRun = value;
    return *this;
}
JsonNotificationBuilder JsonNotificationBuilder::notificationKey(string value)
{
    isNotificationKey = true;
    mNotificationKey = value;
    return *this;
}

JsonNotificationBuilder JsonNotificationBuilder::registrationIds(string value) {
    isRegIds = true;
    mRegIds = value;
    return *this;
}

string JsonNotificationBuilder::build()
{
    stringstream ss;
    ss << "{";
    if (isRegIds) {
        JsonUtil::addPair(&ss, JSON_TAG_REG_IDS, mRegIds);
    }
    if (isNotificationKey) {
        JsonUtil::addPair(&ss, JSON_TAG_NOTIFICATION_KEY, JsonUtil::getJsonString(mNotificationKey));
    }
    /*
    if (isTTL) {
        addPair(&ss, JSON_TAG_TTL, std::to_string(mTimeToLive));
    }
    */
    if (isDelay) {
        JsonUtil::addPair(&ss, JSON_TAG_WHILE_IDLE, mDelayWhileIdle?TRUE:FALSE);
    }

    if (isRestricted) {
        JsonUtil::addPair(&ss, JSON_TAG_RESTRICTED_PACK, JsonUtil::getJsonString(mRestrictedPackageName));
    }

    if (isCollapse) {
        JsonUtil::addPair(&ss, JSON_TAG_COLLAPSE_KEY, JsonUtil::getJsonString(mCollapseKey));
    }

    if (isDryRun) {
        JsonUtil::addPair(&ss, JSON_TAG_DRY_RUN, mDryRun?TRUE:FALSE);
    }

    // DATA section
    if (mKeys.size() > 1) {
        ss << JSON_TAG_DATA << ": {" << endl;
        for(vector<int>::size_type i = 0; i < mKeys.size(); i++) {
            JsonUtil::addPair(&ss, mKeys[i], JsonUtil::getJsonString(mValues[i]));
        }
        ss << "}" << endl;
    }

    ss << "}";
    return ss.str();
}
JsonNotificationBuilder::~JsonNotificationBuilder()
{
    // nothing to do
}

