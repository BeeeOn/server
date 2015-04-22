#include "Utils.h"
#include <sstream>

string Utils::intToString(int i) {
  string number;
  stringstream ss;
  ss << i;
  ss >> number;
  return number;
}
string Utils::longToString(long l) {
  string number;
  stringstream ss;
  ss << l;
  ss >> number;
  return number;
}
