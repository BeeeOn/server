#ifndef BEEEON_RESTUI_SERIALIZING_H
#define BEEEON_RESTUI_SERIALIZING_H

#include <list>
#include <vector>

namespace Poco {
namespace JSON {

class PrintHandler;

}
}

namespace BeeeOn {

class Gateway;
class Location;
class VerifiedIdentity;

namespace RestUI {

void serialize(Poco::JSON::PrintHandler &output, const Gateway &gateway);
void serialize(Poco::JSON::PrintHandler &output,
		const std::vector<Gateway> &gateways);

void serialize(Poco::JSON::PrintHandler &output, const VerifiedIdentity &identity);
void serialize(Poco::JSON::PrintHandler &output,
		const std::list<VerifiedIdentity> &identities);

void serialize(Poco::JSON::PrintHandler &output, const Location &location);
void serialize(Poco::JSON::PrintHandler  &output, const std::vector <Location> &locations);

}

}

#endif
