#include "XmlHelper.h"

void XmlHelper::startTag(stringstream *ss, string tag) {
  (*ss) << "<" << tag << ">";
}

void XmlHelper::endTag(stringstream *ss, string tag) {
  (*ss) << "</" << tag << ">";
}

void XmlHelper::addAttribute(stringstream *ss, string tag, string value) {
  (*ss) << " " << tag << "=\"" << value << "\"";
}

void XmlHelper::tagWithValue(stringstream *ss, string tag, string value) {
  startTag(ss, tag);
  (*ss) << value;
  endTag(ss, tag);
}
