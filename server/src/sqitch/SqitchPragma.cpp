#include "sqitch/SqitchPragma.h"

using namespace std;
using namespace BeeeOn;

SqitchPragma::SqitchPragma(unsigned int line):
	SqitchLine(line)
{
}

void SqitchPragma::setKey(const string &key)
{
	m_key = key;
}

string SqitchPragma::key() const
{
	return m_key;
}

void SqitchPragma::setValue(const string &value)
{
	m_value = value;
}

string SqitchPragma::value() const
{
	return m_value;
}

string SqitchPragma::toString() const
{
	return "%" + key() + " = " + value()
		+ (note().empty()? note() : string());
}
