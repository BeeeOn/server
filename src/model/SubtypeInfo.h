#pragma once

#include "model/CustomTypeID.h"
#include "model/Entity.h"
#include "model/EnumInfo.h"

namespace BeeeOn {

/**
 * @brief SubtypeInfo represents any subtype of a TypeInfo.
 * Subtypes extends certain types with subtype-specific
 * metadata.
 */
class SubtypeInfo : public Entity<CustomTypeID> {
public:
	enum Kind {
		KIND_INVALID,
		KIND_ENUM,
	};

	SubtypeInfo();
	SubtypeInfo(const ID &id, const EnumInfo &enumInfo);

	std::string name() const
	{
		return id().toString();
	}

	EnumInfo &enumInfo();
	const EnumInfo &enumInfo() const;

	Kind kind() const;

private:
	Kind m_kind;
	EnumInfo m_enumInfo;
};

typedef SubtypeInfo::ID SubtypeInfoID;

}
