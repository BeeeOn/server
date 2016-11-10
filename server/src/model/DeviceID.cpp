#include <exception>
#include <sstream>
#include <Poco/Exception.h>
#include <Poco/Random.h>

#include "model/DeviceID.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceID::DeviceID():
	m_value(0)
{
}

DeviceID::DeviceID(uint64_t value):
	m_value(value)
{
	const int shift = is32bit()? 24 : 56;

	if ((m_value >> shift) == 0) {
		throw InvalidArgumentException(
			"no prefix present in the given device ID: "
			+ to_string(m_value));
	}
}

DeviceID::DeviceID(uint8_t prefix, uint64_t ident)
{
	if (ident & 0xff00000000000000UL) {
		throw InvalidArgumentException(
			"ident part of device ID must not overflow 56 bits");
	}

	m_value = ((uint64_t) prefix) << 56;
	m_value |= ident;
}

DeviceID::DeviceID(const DeviceID &copy):
	m_value(copy.m_value)
{
}

DeviceID DeviceID::parse(const string &s)
{
	unsigned long long v;

	try {
		v = stoull(s, NULL, 0);
	}
	catch (invalid_argument &e) {
		throw InvalidArgumentException(
			string("parse device ID: ") + e.what());
	}
	catch (std::out_of_range &e) {
		throw InvalidArgumentException(
			"device ID is out-of-range");
	}

	return DeviceID((uint64_t) v);
}

string DeviceID::toString() const
{
	ostringstream ss;

	ss << "0x" << std::hex;

	if (is32bit())
		ss << (uint32_t) m_value;
	else
		ss << m_value;

	return ss.str();
}

DeviceID DeviceID::random(uint8_t prefix)
{
	Random rnd;
	rnd.seed();

	prefix = prefix? prefix : rnd.next(256);

	uint64_t ident = rnd.next();
	ident <<= 24;
	ident |= rnd.next(1 << 24);

	return DeviceID(prefix, ident);
}
