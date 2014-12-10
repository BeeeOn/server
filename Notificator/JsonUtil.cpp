#include "JsonUtil.h"
#include <sstream>

void JsonUtil::addPair(stringstream *ss, string key, string value) {
    *ss << key << ":" << value << "," << endl;
}

string JsonUtil::getJsonString(string value) {
    return "\"" + value + "\"";
}

string JsonUtil::getJsonArray(vector<string> value) {
    stringstream ss;
    if (value.size() < 1) {
        return "[]";
    }
    ss << "[";
    ss << getJsonString(value[0]);
    unsigned int i;
    for (i = 1; i < value.size(); i++) {
        ss << "," << getJsonString(value[i]);
    }
    ss << "]";
    return ss.str();
}
