#include "sqitch/SqitchEntry.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SqitchEntry::SqitchEntry(unsigned int line):
	SqitchLine(line)
{
}

void SqitchEntry::setName(const string &name)
{
	m_name = name;
}

string SqitchEntry::name() const
{
	return m_name;
}

void SqitchEntry::setTimestamp(const Timestamp &timestamp)
{
	m_timestamp = timestamp;
}

Timestamp SqitchEntry::timestamp() const
{
	return m_timestamp;
}

void SqitchEntry::setAuthor(const string &author)
{
	m_author = author;
}

string SqitchEntry::author() const
{
	return m_author;
}

void SqitchEntry::setAuthorEmail(const string &email)
{
	m_authorEmail = email;
}

string SqitchEntry::authorEmail() const
{
	return m_authorEmail;
}
