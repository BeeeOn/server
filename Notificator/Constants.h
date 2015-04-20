
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>




static const int TTL_UNDEFINED = -1;
static const string STRING_UNDEFINED = "";

/** ACTIONS */
static const string ACTION_DEVICE = "device";


/** DATA TAGS */
static const string DATA_MSGID = "mid";
static const string DATA_TYPE = "type";
static const string DATA_USER_ID = "usrId";
static const string DATA_TIME = "time";
static const string DATA_ACTION = "act";
static const string DATA_MESSAGE = "msg";


/**************************************************/
/*********         JSON              **************/
/**************************************************/

#define TRUE "\"true\""
#define FALSE "\"false\""

/** JSON TAGS */
static const string JSON_TAG_WHILE_IDLE = "\"delay_while_idle\"";
static const string JSON_TAG_NOTIFICATION_KEY = "\"notification_key\"";
static const string JSON_TAG_COLLAPSE_KEY = "\"collapse_key\"";
static const string JSON_TAG_DATA = "\"data\"";
static const string JSON_TAG_TTL = "\"time_to_live\"";
static const string JSON_TAG_RESTRICTED_PACK = "\"restricted_package_name\"";
static const string JSON_TAG_DRY_RUN = "\"dry_run\"";

static const string JSON_TAG_TO = "\"to\"";
static const string JSON_TAG_OPERATION = "\"operation\"";
static const string JSON_TAG_NOTIFICATION_KEY_NAME ="\"notification_key_name\"";
static const string JSON_TAG_REG_IDS = "\"registration_ids\"";

/** JSON DATA KEYS */
static const string JSON_DATA_MSGID = "\"" + DATA_MSGID  + "\"";
static const string JSON_DATA_TYPE = "\"" + DATA_TYPE + "\"";
static const string JSON_DATA_USER_ID = "\"" + DATA_USER_ID + "\"";
static const string JSON_DATA_TIME = "\"" + DATA_TIME + "\"";
static const string JSON_DATA_MESSAGE = "\"" + DATA_MESSAGE + "\"";

#endif
