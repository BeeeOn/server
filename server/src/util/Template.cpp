#include <Poco/String.h>
#include <Poco/StreamCopier.h>

#include "util/Template.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Template::Template(const string &text):
	m_content(text)
{
}

Template::Template(istream &in)
{
	StreamCopier::copyToString(in, m_content);
}

void Template::subst(string &s, const string &key, const string &value)
{
	if (key.empty())
		return;

	replaceInPlace(s, key, value, 0);
}

string Template::apply(const map<string, string> &context)
{
	std::string text(m_content);

	for (auto pair : context)
		subst(text, "${" + pair.first + "}", pair.second);

	return text;
}
