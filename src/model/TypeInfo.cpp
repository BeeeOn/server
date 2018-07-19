#include <cmath>
#include <string>

#include <Poco/Exception.h>
#include <Poco/NumberFormatter.h>

#include "model/TypeInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

TypeInfoID::TypeInfoID():
	m_type(ModuleType::Type::TYPE_UNKNOWN)
{
}

TypeInfoID::TypeInfoID(const ModuleType::Type::Raw &raw):
	m_type(raw)
{
}

TypeInfoID::TypeInfoID(const ModuleType::Type &type):
	m_type(type)
{
}

string TypeInfoID::toString() const
{
	return m_type.toString();
}

TypeInfoID::operator int() const
{
	return m_type;
}

bool TypeInfoID::operator ==(const TypeInfoID &other) const
{
	return m_type == other.m_type;
}

bool TypeInfoID::operator <(const TypeInfoID &other) const
{
	return m_type < other.m_type;
}

bool TypeInfoID::operator <=(const TypeInfoID &other) const
{
	return m_type <= other.m_type;
}

TypeInfoID TypeInfoID::parse(const string &input)
{
	try {
		return TypeInfoID(ModuleType::Type::parse(input));
	}
	catch (const InvalidArgumentException &e) {
		throw NotFoundException("no such type", e);
	}
}

TypeInfo::TypeInfo()
{
}

TypeInfo::TypeInfo(const TypeInfo::ID &id):
	Entity(id)
{
}

string TypeInfo::name() const
{
	return id().toString();
}

void TypeInfo::setUnit(const string &unit)
{
	m_unit = unit;
}

const string &TypeInfo::unit() const
{
	return m_unit;
}

void TypeInfo::setRange(const Range &range)
{
	m_range = range;
}

const TypeInfo::Range &TypeInfo::range() const
{
	return m_range;
}

void TypeInfo::setValues(const map<int, string> &values)
{
	m_values = values;
}

const map<int, string> &TypeInfo::values() const
{
	return m_values;
}

void TypeInfo::setLevels(const set<Level> &levels)
{
	m_levels = levels;
}

const set<TypeInfo::Level> &TypeInfo::levels() const
{
	return m_levels;
}

std::string TypeInfo::asString(const double v) const
{
	return to_string(v);
}

TypeInfo::Range::Range():
	m_min(NAN),
	m_max(NAN),
	m_step(NAN)
{
}

TypeInfo::Range::Range(double min, double max, double step):
	m_min(min),
	m_max(max),
	m_step(step)
{
	if (m_min > m_max)
		throw InvalidArgumentException("min is greater then max");

	if (m_step <= 0)
		throw InvalidArgumentException("step must be positive");
}

bool TypeInfo::Range::hasMin() const
{
	return !std::isnan(m_min);
}

double TypeInfo::Range::min() const
{
	return m_min;
}

bool TypeInfo::Range::hasMax() const
{
	return !std::isnan(m_max);
}

double TypeInfo::Range::max() const
{
	return m_max;
}

bool TypeInfo::Range::hasStep() const
{
	return !std::isnan(m_step);
}

double TypeInfo::Range::step() const
{
	return m_step;
}

bool TypeInfo::Range::isValid() const
{
	return hasStep() || hasMin() || hasMax();
}

TypeInfo::Level::Level():
	m_min(NAN),
	m_max(NAN),
	m_attention(NONE)
{
}

TypeInfo::Level::Level(double value):
	m_min(value),
	m_max(value),
	m_attention(NONE)
{
}

TypeInfo::Level::Level(double min, double max):
	m_min(min),
	m_max(max),
	m_attention(NONE)
{
	if (std::isnan(m_min) && std::isnan(m_max))
		throw InvalidArgumentException("at least min or max must be a number");

	if (!std::isnan(m_min) && !std::isnan(m_max)) {
		if (m_min > m_max)
			throw InvalidArgumentException("min must not be greater then max");
	}
}

bool TypeInfo::Level::lessThan(const TypeInfo::Level &other) const
{
	if (!isValid() && !other.isValid())
		return false;

	if (!isValid())
		return true;

	if (std::isnan(m_max) && std::isnan(other.m_min))
		return false;

	if (m_max < other.m_min)
		return true;

	return false;
}

bool TypeInfo::Level::isValid() const
{
	return !(std::isnan(m_min) && std::isnan(m_max));
}

double TypeInfo::Level::min() const
{
	return m_min;
}

double TypeInfo::Level::max() const
{
	return m_max;
}

void TypeInfo::Level::setAttention(const Attention attention)
{
	m_attention = attention;
}

TypeInfo::Level::Attention TypeInfo::Level::attention() const
{
	return m_attention;
}

void TypeInfo::Level::setLabel(const string &label)
{
	m_label = label;
}

string TypeInfo::Level::label() const
{
	return m_label;
}

TypeInfo::Level::Attention TypeInfo::Level::parseAttention(const string &input)
{
	if (input == "none")
		return NONE;
	else if (input == "single")
		return SINGLE;
	else if (input == "repeat")
		return REPEAT;
	else if (input == "alert")
		return ALERT;

	throw InvalidArgumentException("unexpected attention value: " + input);
}

string TypeInfo::Level::attentionName(const Attention &attention)
{
	switch (attention) {
	case TypeInfo::Level::SINGLE:
		return "single";
	case TypeInfo::Level::REPEAT:
		return "repeat";
	case TypeInfo::Level::ALERT:
		return "alert";
	case TypeInfo::Level::NONE:
		return "none";
	default:
		throw IllegalStateException("invalid attention: " + to_string(attention));
	}
}

bool TypeInfo::operator ==(const ModuleType::Type &type) const
{
	return name() == type.toString();
}

bool TypeInfo::operator !=(const ModuleType::Type &type) const
{
	return name() != type.toString();
}
