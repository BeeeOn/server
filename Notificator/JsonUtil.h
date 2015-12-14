#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <string>
#include <vector>

using namespace std;

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
