#ifndef BEEEON_TYPE_INFO_H
#define BEEEON_TYPE_INFO_H

#include <map>
#include <string>

#include "model/Entity.h"
#include "model/SimpleID.h"

namespace BeeeOn {

class TypeInfo : public Entity<SimpleID> {
public:
	class Range {
	public:
		Range();
		Range(double min, double max, double step);

		bool hasMin() const;
		double min() const;

		bool hasMax() const;
		double max() const;

		bool hasStep() const;
		double step() const;

		bool isValid() const;

	private:
		double m_min;
		double m_max;
		double m_step;
	};

	TypeInfo();
	TypeInfo(const TypeInfo::ID &id);

	void setName(const std::string &name);
	const std::string &name() const;

	void setUnit(const std::string &unit);
	const std::string &unit() const;

	void setRange(const Range &range);
	const Range &range() const;

	void setValues(const std::map<int, std::string> &values);
	const std::map<int, std::string> &values() const;

	std::string asString(const double v) const;

private:
	std::string m_name;
	std::string m_unit;
	Range m_range;
	std::map<int, std::string> m_values;
};

typedef TypeInfo::ID TypeInfoID;

}

#endif
