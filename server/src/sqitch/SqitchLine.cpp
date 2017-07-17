#include "sqitch/SqitchLine.h"

using namespace std;
using namespace BeeeOn;

SqitchLine::SqitchLine(unsigned int line):
	m_line(line)
{
}

SqitchLine::~SqitchLine()
{
}

unsigned int SqitchLine::line() const
{
	return m_line;
}

void SqitchLine::setNote(const string &note)
{
	m_note = note;
}

string SqitchLine::note() const
{
	return m_note;
}

bool SqitchLine::equals(const SqitchLine &other) const
{
	return m_line == other.m_line;
}

bool SqitchLine::lessThen(const SqitchLine &other) const
{
	return m_line < other.m_line;
}

string SqitchLine::toString() const
{
	return "# " + m_line;
}
