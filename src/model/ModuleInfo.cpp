#include "model/ModuleInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

ModuleInfo::ModuleInfo():
	m_controllable(false)
{
}

void ModuleInfo::setType(const SharedPtr<TypeInfo> type)
{
	m_type = type;
}

const SharedPtr<TypeInfo> ModuleInfo::type() const
{
	return m_type;
}

void ModuleInfo::setSubtype(const SharedPtr<SubtypeInfo> subtype)
{
	m_subtype = subtype;
}

const SharedPtr<SubtypeInfo> ModuleInfo::subtype() const
{
	return m_subtype;
}

void ModuleInfo::setName(const string &name)
{
	m_name = name;
}

const string &ModuleInfo::name() const
{
	return m_name;
}

void ModuleInfo::setGroup(const string &group)
{
	m_group = group;
}

const string &ModuleInfo::group() const
{
	return m_group;
}

void ModuleInfo::setControllable(bool controllable)
{
	m_controllable = controllable;
}

bool ModuleInfo::isControllable() const
{
	return m_controllable;
}
void ModuleInfo::setFromUnknown(const string &formula)
{
	m_fromUnknown = formula;
}

string ModuleInfo::fromUnknown() const
{
	return m_fromUnknown;
}

void ModuleInfo::setToUnknown(const string &formula)
{
	m_toUnknown = formula;
}

string ModuleInfo::toUnknown() const
{
	return m_toUnknown;
}

void ModuleInfo::assureCompatible(const ModuleType &type) const
{
	if (isControllable()) {
		if (!type.isControllable()) {
			throw InvalidArgumentException(
				"type " + type.type().toString()
				+ " expected to be controllable");
		}
	}
	else {
		if (type.isControllable()) {
			throw InvalidArgumentException(
				"type " + type.type().toString()
				+ " is not expected to be controllable");
		}
	}

	if (type.type() == ModuleType::Type::TYPE_UNKNOWN) {
		if (type.isControllable()) {
			if (m_fromUnknown.empty()) {
				throw InvalidArgumentException(
					"type is controllable unknown but no way to"
					" convert from unknown is defined");
			}
			if (m_toUnknown.empty()) {
				throw InvalidArgumentException(
					"type is controllable unknown but no way to"
					" convert to unknown is defined");
			}

			return;
		}
		else {
			if (m_fromUnknown.empty()) {
				throw InvalidArgumentException(
					"type is unknown but no way to"
					" convert from unknown is defined");
			}

			return;
		}
	}

	if (subtype().isNull() || subtype()->kind() == SubtypeInfo::KIND_INVALID) {
		if (!type.customTypeID().isNull()) {
			throw InvalidArgumentException(
				"type " + type.type().toString()
				+ " is not expected to have subtype");
		}
	}
	else {
		if (type.customTypeID().isNull()) {
			throw InvalidArgumentException(
				"type " + type.type().toString()
				+ " is expected to have subtype");
		}

		if (subtype()->name() != type.customTypeID().toString()) {
			throw InvalidArgumentException(
				"subtype " + subtype()->name()
				+ " is incompatible with " + type.customTypeID().toString());
		}
	}

	if (*m_type != type.type()) {
		throw InvalidArgumentException(
			"type " + toString() + " is incompatible with "
			+ type.type().toString());
	}
}

const string ModuleInfo::toString() const
{
	string buffer;

	if (type().isNull())
		buffer.append("(null)");
	else
		buffer.append(type()->name());

	if (subtype().isNull())
		buffer.append(",(null)");
	else if (subtype()->kind() == SubtypeInfo::KIND_INVALID)
		buffer.append(",none");
	else
		buffer.append(subtype()->name());

	if (isControllable())
		buffer.append(",controllable");

	return buffer;
}
