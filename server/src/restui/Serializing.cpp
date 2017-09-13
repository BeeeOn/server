#include <cmath>

#include <Poco/DateTime.h>
#include <Poco/JSON/PrintHandler.h>

#include "l10n/Translator.h"
#include "model/Device.h"
#include "model/Gateway.h"
#include "model/LegacyRoleInGateway.h"
#include "model/Location.h"
#include "model/RoleInGateway.h"
#include "model/User.h"
#include "model/VerifiedIdentity.h"
#include "restui/Serializing.h"
#include "work/Work.h"

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

void BeeeOn::RestUI::serialize(PrintHandler &output, const Work &work)
{
	output.startObject();

	output.key("id");
	output.value(work.id().toString());

	output.key("status");

	switch (work.state()) {
	case Work::STATE_IDLE:
		output.value("idle");
		break;
	case Work::STATE_SCHEDULED:
		output.value("sleeping");
		break;
	case Work::STATE_EXECUTED:
		output.value("processing");
		break;
	case Work::STATE_FINISHED:
		output.value("successful");
		break;
	case Work::STATE_FAILED:
		output.value("failed");
		break;
	case Work::STATE_CANCELED:
		output.value("canceled");
		break;
	default:
		throw IllegalStateException("invalid work state");
	}

	output.key("created");
	output.value(work.created().epochTime());

	if (Work::timestampValid(work.finished())) {
		output.key("finished");
		output.value(work.finished().epochTime());
	}

	Nullable<Timestamp> activationTime = work.activationTime();
	if (!activationTime.isNull()) {
		output.key("next_wakeup");
		output.value(activationTime.value().epochTime());
	}

	output.key("owner_id");
	output.value(work.owner().id().toString());

	output.endObject();
}

void BeeeOn::RestUI::serialize(PrintHandler &output, const Device &device)
{
	output.startObject();

	output.key("id");
	output.value(device.id().toString());
	output.key("location_id");
	output.value(device.location().id().toString());
	output.key("name");
	output.value(device.name());

	output.key("type");
	output.startObject();
	output.key("name");
	output.value(device.type().get()->name());
	output.key("display_name");
	output.value(device.type().get()->displayName());
	output.key("vendor");
	output.value(device.type().get()->vendor());
	output.endObject();

	output.key("first_seen");
	output.value(device.firstSeen().timestamp().epochTime());
	output.key("last_seen");
	output.value(device.lastSeen().timestamp().epochTime());

	if (!device.activeSince().isNull()) {
		output.key("active_since");
		output.value(device.activeSince().value().timestamp().epochTime());
	}

	output.key("refresh_time");
	output.value(device.refresh().totalSeconds());
	output.key("available");
	output.value(device.available());

	output.endObject();
}

void BeeeOn::RestUI::serialize(PrintHandler &output, const std::vector<RoleInGateway> &roles)
{
	output.startArray();

	for (auto role : roles)
		serialize(output, role);

	output.endArray();
}

void BeeeOn::RestUI::serialize(Poco::JSON::PrintHandler &output,
		const std::vector<Device> &devices)
{
	output.startArray();

	for (auto &device : devices)
		serialize(output, device);

	output.endArray();
}

void BeeeOn::RestUI::serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const TypeInfo &info)
{
	output.startObject();

	output.key("id");
	output.value(info.id().toString());

	output.key("name");
	output.value(translator.format("types." + info.name()));

	if (!info.unit().empty()) {
		output.key("unit");
		output.value(info.unit());
	}

	output.endObject();
}

void BeeeOn::RestUI::serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const TypeInfoProvider::InfoSet::const_iterator begin,
		const TypeInfoProvider::InfoSet::const_iterator end)
{
	output.startArray();

	for (auto it = begin; it != end; ++it) {
		SharedPtr<TypeInfo> info = *it;
		serialize(output, translator, *info);
	}

	output.endArray();
}
