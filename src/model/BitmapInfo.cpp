#include <Poco/Exception.h>

#include "model/BitmapInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BitmapInfo::Bits::Bits()
{
}

void BitmapInfo::Bits::setName(const string &name)
{
	m_name = name;
}

string BitmapInfo::Bits::name() const
{
	return m_name;
}

BitmapInfo::Flag::Flag():
	m_index(-1),
	m_attention(TypeInfo::Level::NONE),
	m_inversed(false)
{
}

BitmapInfo::Flag::Flag(uint8_t index, bool inversed):
	m_index(-1),
	m_inversed(inversed)
{
	setIndex(index);
}

void BitmapInfo::Flag::setIndex(uint8_t index)
{
	if (index >= 8 * sizeof(uint32_t))
		throw InvalidArgumentException("flag index out-of-range");

	m_index = index;
}

uint8_t BitmapInfo::Flag::index() const
{
	if (m_index < 0)
		throw IllegalStateException("flag index is invalid");

	return m_index;
}

void BitmapInfo::Flag::setAttention(TypeInfo::Level::Attention attention)
{
	m_attention = attention;
}

TypeInfo::Level::Attention BitmapInfo::Flag::attention() const
{
	return m_attention;
}

void BitmapInfo::Flag::setInversed(bool inversed)
{
	m_inversed = inversed;
}

bool BitmapInfo::Flag::inversed() const
{
	return m_inversed;
}

uint32_t BitmapInfo::Flag::extract(const uint32_t value)
{
	const uint32_t b = (value >> m_index) & 1;

	if (m_inversed)
		return !b;

	return b;
}

BitmapInfo::Group::Group()
{
}

BitmapInfo::Group::Group(const vector<uint8_t> &bits)
{
	setBits(bits);
}

void BitmapInfo::Group::addBit(uint8_t index)
{
	if (index >= 8 * sizeof(uint32_t))
		throw InvalidArgumentException("group bit out-of-range");

	m_bits.emplace_back(index);
}

void BitmapInfo::Group::setBits(const vector<uint8_t> &bits)
{
	m_bits.clear();

	for (const auto &index : bits)
		addBit(index);
}

const vector<uint8_t> &BitmapInfo::Group::bits() const
{
	return m_bits;
}

uint32_t BitmapInfo::Group::extract(const uint32_t value)
{
	uint32_t result = 0;
	uint8_t i = 0;

	for (const auto index : m_bits) {
		const uint32_t b = (value >> index) & 1;
		result |= (b << i);
		++i;
	}

	return result;
}

bool BitmapInfo::Group::operator <(const Group &other) const
{
	return m_bits < other.m_bits;
}

bool BitmapInfo::Group::operator >(const Group &other) const
{
	return m_bits > other.m_bits;
}

bool BitmapInfo::Group::operator ==(const Group &other) const
{
	return m_bits == other.m_bits;
}

bool BitmapInfo::Group::operator !=(const Group &other) const
{
	return m_bits != other.m_bits;
}

BitmapInfo::BitmapInfo()
{
}

void BitmapInfo::setFlags(const map<uint8_t, Flag> &flags)
{
	m_flags = flags;
}

const map<uint8_t, BitmapInfo::Flag> &BitmapInfo::flags() const
{
	return m_flags;
}

void BitmapInfo::setGroups(const set<Group> &groups)
{
	m_groups = groups;
}

const set<BitmapInfo::Group> &BitmapInfo::groups() const
{
	return m_groups;
}

vector<BitmapInfo::BitsRef> BitmapInfo::bits() const
{
	vector<BitsRef> bits;

	for (const auto &pair : m_flags)
		bits.emplace_back(static_cast<const Bits &>(pair.second));
	for (const auto &group : m_groups)
		bits.emplace_back(static_cast<const Bits &>(group));

	return bits;
}
