#include <Poco/TextEncoding.h>
#include <Poco/TextConverter.h>
#include <Poco/UTF8Encoding.h>

#include "util/Sanitize.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

string Sanitize::encoding(const string &bytes,
		const unsigned long sizeLimit,
		const string &inputEncoding)
{
	const TextEncoding &encoding = TextEncoding::byName(inputEncoding);
	const UTF8Encoding utf8;

	if (sizeLimit > 0 && bytes.size() > sizeLimit)
		throw InvalidArgumentException("input is too long");

	TextConverter text(encoding, utf8);
	string result;
	text.convert(bytes, result);

	return result;
}
