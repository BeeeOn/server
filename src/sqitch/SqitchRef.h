#ifndef BEEEON_SQITCH_REF_H
#define BEEEON_SQITCH_REF_H

#include <string>

namespace BeeeOn {

class SqitchRef {
public:
	SqitchRef(const std::string &input);
	SqitchRef(
		const std::string &project,
		const std::string &name,
		const std::string &tag);

	std::string project(const std::string &current = "") const;
	std::string name() const;
	std::string tag(const std::string &last = "") const;

	std::string toString() const;

	SqitchRef forProject(const std::string &project) const;

	bool equals(const SqitchRef &other) const;
	bool lessThen(const SqitchRef &other) const;

	bool operator ==(const SqitchRef &other) const
	{
		return equals(other);
	}

	bool operator !=(const SqitchRef &other) const
	{
		return !equals(other);
	}

	bool operator <(const SqitchRef &other) const
	{
		return lessThen(other);
	}

	bool operator <=(const SqitchRef &other) const
	{
		return !other.lessThen(*this);
	}

	bool operator >(const SqitchRef &other) const
	{
		return other.lessThen(*this);
	}

	bool operator >=(const SqitchRef &other) const
	{
		return !lessThen(other);
	}

private:
	void parse(const std::string &input);

private:
	std::string m_project;
	std::string m_name;
	std::string m_tag;
};

}

#endif
