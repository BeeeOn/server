#ifndef XMLHELPER_H
#define XMLHELPER_H

#include <string>
#include <sstream>

using namespace std;

class XmlHelper {
public:
    static void startTag(stringstream *ss, string tag);
    static void endTag(stringstream *ss, string tag);
    static void addAttribute(stringstream *ss, string tag, string value);
    static void tagWithValue(stringstream *ss, string tag, string value);
private:
    XmlHelper() {
    };
};

#endif // XMLHELPER_H
