#pragma once

#include <map>
#include <set>
#include <string>

#include "model/Entity.h"
#include "model/ModuleType.h"

namespace BeeeOn {

class TypeInfoID {
public:
	TypeInfoID();
	TypeInfoID(const ModuleType::Type::Raw &raw);
	TypeInfoID(const ModuleType::Type &type);

	std::string toString() const;
	operator int() const;

	bool operator ==(const TypeInfoID &other) const;
	bool operator <(const TypeInfoID &other) const;
	bool operator <=(const TypeInfoID &other) const;

	bool operator !=(const TypeInfoID &other) const
	{
		return !(m_type == other.m_type);
	}

	bool operator >(const TypeInfoID &other) const
	{
		return other.m_type < m_type;
	}

	bool operator >=(const TypeInfoID &other) const
	{
		return other.m_type <= m_type;
	}

	static TypeInfoID parse(const std::string &input);

private:
	ModuleType::Type m_type;
};

class TypeInfo : public Entity<TypeInfoID> {
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

	class Level {
	public:
		enum Attention {
			/**
			 * Do not notify user about this level.
			 */
			NONE,
			/**
			 * Notify the user gently once.
			 */
			SINGLE,
			/**
			 * Notify the user gently and repeat if the level does not improve.
			 */
			REPEAT,
			/**
			 * Notify the user loudly.
			 * The level is serious and can be dangerous.
			 */
			ALERT
		};

		Level();
		Level(double value);
		Level(double min, double max);

		bool operator <(const Level &other) const
		{
			return lessThan(other);
		}

		bool lessThan(const Level &other) const;

		bool isValid() const;

		double min() const;
		double max() const;

		void setAttention(const Attention attention);
		Attention attention() const;

		void setLabel(const std::string &label);
		std::string label() const;

		static Attention parseAttention(const std::string &input);
		static std::string attentionName(const Attention &attention);

	private:
		double m_min;
		double m_max;
		Attention m_attention;
		std::string m_label;
	};

	TypeInfo();
	TypeInfo(const TypeInfo::ID &id);

	std::string name() const;

	void setUnit(const std::string &unit);
	const std::string &unit() const;

	void setRange(const Range &range);
	const Range &range() const;

	void setValues(const std::map<int, std::string> &values);
	const std::map<int, std::string> &values() const;

	void setLevels(const std::set<Level> &levels);
	const std::set<Level> &levels() const;

	std::string asString(const double v) const;

	bool operator ==(const ModuleType::Type &type) const;
	bool operator !=(const ModuleType::Type &type) const;

private:
	std::string m_unit;
	Range m_range;
	std::map<int, std::string> m_values;
	std::set<Level> m_levels;
};

}
