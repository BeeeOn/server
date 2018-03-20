#pragma once

#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "model/TypeInfo.h"

namespace BeeeOn {

/**
 * @brief BitmapInfo describes a specific bitmap data type.
 * It describes structure of a bitmap that consists of:
 *
 * - flag bits - single bit denoting some state
 * - group bits - a group of bits that make sense together
 */
class BitmapInfo {
public:
	class Bits {
	public:
		Bits();

		void setName(const std::string &name);
		std::string name() const;

		virtual uint32_t extract(const uint32_t value) = 0;

	private:
		std::string m_name;
	};

	typedef std::reference_wrapper<const Bits> BitsRef;

	/**
	 * @brief Flag represents a single bit of a bitmap.
	 * It describes its purpose (by name) and its index in the bitmap.
	 * The bit can be active high or active low (inversed). If the
	 * bit is active, its attention is to be considered.
	 */
	class Flag : public Bits {
	public:
		Flag();
		Flag(uint8_t index, bool inversed = false);

		void setIndex(uint8_t index);
		uint8_t index() const;

		void setAttention(TypeInfo::Level::Attention attention);
		TypeInfo::Level::Attention attention() const;

		void setInversed(bool inversed);
		bool inversed() const;

		uint32_t extract(const uint32_t value) override;

	private:
		int m_index;
		TypeInfo::Level::Attention m_attention;
		bool m_inversed;
	};

	/**
	 * @brief Group represents a number consisting of selected
	 * bits of the bitmap type. Picking the group bits from the
	 * given raw value creates a number consisting of only group
	 * bits.
	 *
	 * Example:
	 *
	 * - group: {0, 2, 1}
	 * - input: 0b1010
	 * - output: 0b100 (mapping: 0->0 [0], 1->2 [1], 2->1 [0])
	 */
	class Group : public Bits {
	public:
		Group();
		Group(const std::vector<uint8_t> &bits);

		void addBit(uint8_t index);
		void setBits(const std::vector<uint8_t> &bits);
		const std::vector<uint8_t> &bits() const;

		uint32_t extract(const uint32_t value) override;

		bool operator<(const Group &other) const;
		bool operator>(const Group &other) const;
		bool operator==(const Group &other) const;
		bool operator!=(const Group &other) const;

	private:
		std::vector<uint8_t> m_bits;
	};

	BitmapInfo();

	void setFlags(const std::map<uint8_t, Flag> &flags);
	const std::map<uint8_t, Flag> &flags() const;

	void setGroups(const std::set<Group> &groups);
	const std::set<Group> &groups() const;

	std::vector<BitsRef> bits() const;

private:
	std::map<uint8_t, Flag> m_flags;
	std::set<Group> m_groups;
};

}
