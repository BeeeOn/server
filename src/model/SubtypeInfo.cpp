#include <Poco/Exception.h>

#include "model/SubtypeInfo.h"

using namespace Poco;
using namespace BeeeOn;

SubtypeInfo::SubtypeInfo():
	m_kind(KIND_INVALID)
{
}

SubtypeInfo::SubtypeInfo(
		const ID &id,
		const EnumInfo &enumInfo):
	Entity<CustomTypeID>(id),
	m_kind(KIND_ENUM),
	m_enumInfo(enumInfo)
{
}

EnumInfo &SubtypeInfo::enumInfo()
{
	if (m_kind != KIND_ENUM) {
		throw IllegalStateException(
			"accessing EnumInfo of non-enum subtype");
	}

	return m_enumInfo;
}

const EnumInfo &SubtypeInfo::enumInfo() const
{
	if (m_kind != KIND_ENUM) {
		throw IllegalStateException(
			"accessing EnumInfo of non-enum subtype");
	}

	return m_enumInfo;
}

SubtypeInfo::Kind SubtypeInfo::kind() const
{
	return m_kind;
}
