#include <Poco/Ascii.h>
#include <Poco/Random.h>
#include <Poco/Exception.h>

#include "util/DAMM.h"
#include "model/GatewayID.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GatewayID::GatewayID(int version, uint64_t data)
{
	if (version <= 0 || version >= 10) {
		throw InvalidArgumentException(
			"invalid version, must be: 1 < version < 10");
	}

	string tmp;
	tmp.append(to_string(version));
	tmp.append(to_string(data));
	tmp.append(to_string(DAMM::compute(tmp)));

	m_value = stol(tmp);
}

GatewayID GatewayID::parse(const string &s)
{
	if (s.size() != GatewayID::LENGTH) {
		throw InvalidArgumentException(
			"expected string of length "
			+ std::to_string(LENGTH));
	}

	if (!Ascii::isDigit(s.at(0))) {
		throw InvalidArgumentException(
			"version must be a decimal digit");
	}

	const char version = s.at(0) - '0';
	if (version <= 0 || version >= 10) {
		throw InvalidArgumentException(
			"invalid version, must be: 1 < version < 10");
	}

	const long data = stol(s.substr(1, s.size() - 2));
	if (data < 0) {
		throw InvalidArgumentException(
			"invalid data, must not be negative");
	}

	if (!Ascii::isDigit(s.at(s.size() - 1))) {
		throw InvalidArgumentException(
			"check digit must be a decimal digit");
	}

	const int sum = s.at(s.size() - 1) - '0';
	const int result = DAMM::compute(s.substr(0, s.size() - 1));
	if (result != sum)
		throw InvalidArgumentException("invalid checksum");

	return GatewayID(version, data);
}

GatewayID GatewayID::random(int version)
{
	Random rnd;
	rnd.seed();

	uint64_t data = 0;

	for (int i = 0; i < LENGTH - 2; ++i) {
		int c = rnd.nextChar();
		data *= 10;
		data += c;
	}

	return GatewayID(version, data);
}
