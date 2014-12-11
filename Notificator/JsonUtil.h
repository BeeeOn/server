#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <string>
#include <vector>

#define TRUE "\"true\""
#define FALSE "\"false\""

using namespace std;

static const int TTL_UNDEFINED = -1;
static const bool DEFAULT_DELAY_IDLE = false;
static const bool DEFAULT_DRY_RUN = false;
static const string STRING_UNDEFINED = "";

/** JSON TAGS */
static const string TAG_WHILE_IDLE = "\"delay_while_idle\"";
static const string TAG_NOTIFICATION_KEY = "\"notification_key\"";
static const string TAG_COLLAPSE_KEY = "\"collapse_key\"";
static const string TAG_DATA = "\"data\"";
static const string TAG_TTL = "\"time_to_live\"";
static const string TAG_RESTRICTED_PACK = "\"restricted_package_name\"";
static const string TAG_DRY_RUN = "\"dry_run\"";

static const string TAG_TO = "\"to\"";
static const string TAG_OPERATION = "\"operation\"";
static const string TAG_NOTIFICATION_KEY_NAME ="\"notification_key_name\"";
static const string TAG_REG_IDS = "\"registration_ids\"";

/** JSON DATA KEYS */
static const string DATA_MSGID = "\"mid\"";
static const string DATA_TYPE = "\"type\"";
static const string DATA_EMAIL = "\"email\"";
static const string DATA_TIME = "\"time\"";
static const string DATA_ACTION = "\"act\"";
static const string DATA_MESSAGE = "\"msg\"";

class JsonUtil
{
    public:
        static void addPair(stringstream *ss, string key, string value);
        static string getJsonString(string value);
        static string getJsonArray(vector<string> value);
    protected:
    private:
        JsonUtil();
};

#endif // JSONUTIL_H
