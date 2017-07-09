#include <cmath>

#include <Poco/DateTime.h>
#include <Poco/JSON/PrintHandler.h>

#include "model/Gateway.h"
#include "model/LegacyRoleInGateway.h"
#include "model/Location.h"
#include "model/RoleInGateway.h"
#include "model/User.h"
#include "model/VerifiedIdentity.h"
#include "restui/Serializing.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

static void valueDouble(PrintHandler &output, double d)
{
	if (std::isnan(d) || std::isinf(d))
		output.null();
	else
		output.value(d);
}

static void gatewayStatus(PrintHandler &output, const Gateway &gateway)
{
	output.startObject();

	output.key("lastModified");

	if (gateway.lastChanged().isNull())
		output.null();
	else {
		const DateTime &dt = gateway.lastChanged();
		output.value(dt.timestamp().epochTime());
	}

	output.key("ip");
	output.value(gateway.ipAddress().toString());

	output.key("version");
	output.value(gateway.version());

	output.endObject();
}

void BeeeOn::RestUI::serialize(PrintHandler &output, const Gateway &gateway)
{
	output.startObject();

	output.key("id");
	output.value(gateway.id().toString());
	output.key("name");
	output.value(gateway.name());
	output.key("altitude");

	if (gateway.altitude().isNull())
		output.value(gateway.altitude().value());
	else
		output.null();

	output.key("latitude");
	valueDouble(output, gateway.latitude());
	output.key("longitude");
	valueDouble(output, gateway.longitude());

	output.key("state");
	gatewayStatus(output, gateway);

	output.endObject();
}

void BeeeOn::RestUI::serialize(PrintHandler &output,
		const vector<Gateway> &gateways)
{
	output.startArray();

	for (auto &gateway : gateways)
		serialize(output, gateway);

	output.endArray();
}

void BeeeOn::RestUI::serialize(PrintHandler &output, const VerifiedIdentity &identity)
{
	const User &user = identity.user();
	output.startObject();

	output.key("id");
	output.value(identity.id().toString());

	output.key("first_name");
	output.value(user.firstName());

	output.key("last_name");
	output.value(user.lastName());

	output.key("email");
	output.value(identity.email());

	output.key("picture_url");
	output.value(identity.picture().toString());

	output.key("provider");
	output.value(identity.provider());

	output.endObject();
}

void BeeeOn::RestUI::serialize(PrintHandler &output,
		const list<VerifiedIdentity> &identities)
{
	output.startArray();

	for (auto &identity : identities)
		serialize(output, identity);

	output.endArray();
}

void BeeeOn::RestUI::serialize(PrintHandler &output, const Location &location)
{
	output.startObject();
	output.key("id");
	output.value(location.id().toString());

	output.key("name");
	output.value(location.name());

	output.key("type");
	output.value(0); //TODO: Location does not have a type
	output.endObject();
}

void BeeeOn::RestUI::serialize(PrintHandler &output, const std::vector<Location> &locations)
{
	output.startArray();

	for (auto location : locations)
		serialize(output, location);

	output.endArray();
}

static void roleCommon(PrintHandler &output, const RoleInGateway &role)
{
	output.key("id");
	output.value(role.id().toString());

	output.key("identity_email");
	output.value(role.identity().email());

	output.key("access_level");
	output.value(role.level().toString());

	output.key("created");
	output.value(role.created().timestamp().epochTime());
}

void BeeeOn::RestUI::serialize(PrintHandler &output, const RoleInGateway &role)
{
	output.startObject();

	roleCommon(output, role);

	output.endObject();
}

void BeeeOn::RestUI::serialize(PrintHandler &output, const LegacyRoleInGateway &role)
{
	output.startObject();

	roleCommon(output, role);

	output.key("user");
	output.startObject();

	output.key("first_name");
	output.value(role.firstName());

	output.key("last_name");
	output.value(role.lastName());

	output.endObject();

	output.endObject();
}

void BeeeOn::RestUI::serialize(PrintHandler &output, const std::vector<RoleInGateway> &roles)
{
	output.startArray();

	for (auto role : roles)
		serialize(output, role);

	output.endArray();
}
