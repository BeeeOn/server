#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>

#include "sqitch/SqitchTag.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SqitchTag::SqitchTag(unsigned int line):
	SqitchEntry(line)
{
}

string SqitchTag::toString() const
{
	string result = "@" + name() + " ";

	DateTimeFormatter::append(
		result,
		timestamp(),
		DateTimeFormat::ISO8601_FORMAT
	);

	result += " ";
	result += author();
	result += " <";
	result += authorEmail();
	result += ">";

	if (!note().empty()) {
		result += " # ";
		result += note();
	}

	return result;
}
