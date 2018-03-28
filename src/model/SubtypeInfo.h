#pragma once

#include "model/CustomTypeID.h"
#include "model/Entity.h"
#include "model/EnumInfo.h"
#include "model/BitmapInfo.h"

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
		KIND_BITMAP,
	};

	SubtypeInfo();
	SubtypeInfo(const ID &id, const EnumInfo &enumInfo);
	SubtypeInfo(const ID &id, const BitmapInfo &bitmapInfo);

	std::string name() const
	{
		return id().toString();
	}

	EnumInfo &enumInfo();
	const EnumInfo &enumInfo() const;

	BitmapInfo &bitmapInfo();
	const BitmapInfo &bitmapInfo() const;

	Kind kind() const;

private:
	Kind m_kind;
	EnumInfo m_enumInfo;
	BitmapInfo m_bitmapInfo;
};

typedef SubtypeInfo::ID SubtypeInfoID;

}
