#pragma once

#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>

namespace BeeeOn {

class SqitchLine {
public:
	typedef Poco::SharedPtr<SqitchLine> Ptr;

	SqitchLine(unsigned int line);
	virtual ~SqitchLine();

	unsigned int line() const;

	virtual bool equals(const SqitchLine &other) const;
	virtual bool lessThen(const SqitchLine &other) const;

	void setNote(const std::string &note);
	std::string note() const;

	virtual std::string toString() const;

	bool operator ==(const SqitchLine &other)
	{
		return equals(other);
	}

	bool operator !=(const SqitchLine &other)
	{
		return !equals(other);
	}

	bool operator <(const SqitchLine &other) const
	{
		return lessThen(other);
	}

	bool operator <=(const SqitchLine &other) const
	{
		return !other.lessThen(*this);
	}

	bool operator >(const SqitchLine &other) const
	{
		return other.lessThen(*this);
	}

	bool operator >=(const SqitchLine &other) const
	{
		return !lessThen(other);
	}

private:
	unsigned int m_line;
	std::string m_note;
};

}
