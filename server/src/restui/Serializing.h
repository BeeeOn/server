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
class LegacyRoleInGateway;
class Location;
class VerifiedIdentity;
class RoleInGateway;
class Work;

namespace RestUI {

void serialize(Poco::JSON::PrintHandler &output, const Gateway &gateway);
void serialize(Poco::JSON::PrintHandler &output,
		const std::vector<Gateway> &gateways);

void serialize(Poco::JSON::PrintHandler &output, const VerifiedIdentity &identity);
void serialize(Poco::JSON::PrintHandler &output,
		const std::list<VerifiedIdentity> &identities);

void serialize(Poco::JSON::PrintHandler &output, const Location &location);
void serialize(Poco::JSON::PrintHandler  &output, const std::vector <Location> &locations);

void serialize(Poco::JSON::PrintHandler &output, const RoleInGateway &role);
void serialize(Poco::JSON::PrintHandler &output, const LegacyRoleInGateway &role);
void serialize(Poco::JSON::PrintHandler  &output, const std::vector <RoleInGateway> &roles);

void serialize(Poco::JSON::PrintHandler &output, const Work &work);

}

}

#endif
