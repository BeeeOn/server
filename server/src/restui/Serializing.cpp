#include <cmath>

#include <Poco/DateTime.h>
#include <Poco/JSON/PrintHandler.h>

#include "l10n/Translator.h"
#include "model/Device.h"
#include "model/Gateway.h"
#include "model/LegacyRoleInGateway.h"
#include "model/Location.h"
#include "model/ModuleInfo.h"
#include "model/RoleInGateway.h"
#include "model/Sensor.h"
#include "model/User.h"
#include "model/VerifiedIdentity.h"
#include "restui/RestValueConsumer.h"
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

void BeeeOn::RestUI::serialize(PrintHandler &output, const list<Work> &works)
{
	output.startArray();

	for (auto &work : works)
		serialize(output, work);

	output.endArray();
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
	output.value(translator.format("types." + info.name() + ".label", info.name()));

	if (!info.unit().empty()) {
		output.key("unit");
		output.value(info.unit());
	}

	const TypeInfo::Range &range = info.range();
	if (range.isValid()) {
		output.key("range");
		output.startObject();

		if (range.hasMin()) {
			output.key("min");
			output.value(range.min());
		}

		if (range.hasMax()) {
			output.key("max");
			output.value(range.max());
		}

		if (range.hasStep()) {
			output.key("step");
			output.value(range.step());
		}

		output.endObject();
	}

	if (!info.values().empty()) {
		output.key("values");
		output.startObject();

		for (const auto pair : info.values()) {
			output.key(to_string(pair.first));
			output.value(translator.format(
				"types." + info.name() + ".values." + pair.second,
				pair.second));
		}

		output.endObject();
	}

	if (!info.levels().empty()) {
		output.key("levels");
		output.startArray();

		for (const auto level : info.levels()) {
			if (!level.isValid())
				continue; // should not happen but...

			output.startObject();

			if (!level.label().empty()) {
				output.key("name");
				output.value(translator.format(
					"types." + info.name() + ".levels." + level.label(),
					level.label()));
			}

			if (!std::isnan(level.min())) {
				output.key("min");
				output.value(level.min());
			}

			if (!std::isnan(level.max())) {
				output.key("max");
				output.value(level.max());
			}

			switch (level.attention()) {
			case TypeInfo::Level::SINGLE:
				output.key("attention");
				output.value(string("single"));
				break;

			case TypeInfo::Level::REPEAT:
				output.key("attention");
				output.value(string("repeat"));
				break;

			case TypeInfo::Level::ALERT:
				output.key("attention");
				output.value(string("alert"));
				break;

			default:
				break;
			}

			output.endObject();
		}

		output.endArray();
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

void BeeeOn::RestUI::serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const Sensor &sensor)
{
	const ModuleInfo &info = sensor.info();

	output.startObject();

	output.key("id");
	output.value(sensor.id().toString());

	if (!sensor.name().empty()) {
		output.key("display_name");
		output.value(sensor.name());
	}
	else if (!info.name().empty()) {
		output.key("display_name");
		output.value(translator.format(
			"modules." + info.name() + ".label", info.name()));
	}
	else {
		output.key("display_name");
		output.value(translator.format(
			"types." + info.type()->name() + ".label", info.type()->name()));
	}

	output.key("type_id");
	output.value(info.type()->id().toString());

	if (!info.group().empty()) {
		output.key("group");
		output.value(translator.format(
			"modules.groups." + info.group() + ".label", info.group()));
	}

	output.key("order");
	output.value(info.id().toString());

	if (!sensor.at().isNull()) {
		output.key("current");

		RestValueConsumer consumer(output);
		consumer.begin(*sensor.info().type());
		consumer.single(ValueAt(sensor.at().value(), sensor.value()));
		consumer.end();
	}

	output.endObject();
}

void BeeeOn::RestUI::serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const list<Sensor> &sensors)
{
	output.startArray();

	for (auto &sensor : sensors)
		serialize(output, translator, sensor);

	output.endArray();
}

void BeeeOn::RestUI::serialize(PrintHandler &output,
		Translator &translator,
		const Control &control)
{
	const ModuleInfo &info = control.info();

	output.startObject();

	output.key("id");
	output.value(control.id().toString());

	if (!control.name().empty()) {
		output.key("display_name");
		output.value(control.name());
	}
	else if (!info.name().empty()) {
		output.key("display_name");
		output.value(translator.format(
			"modules." + info.name() + ".label", info.name()));
	}
	else {
		output.key("display_name");
		output.value(translator.format(
			"types." + info.type()->name() + ".label", info.type()->name()));
	}

	output.key("type_id");
	output.value(info.type()->id().toString());

	if (!info.group().empty()) {
		output.key("group");
		output.value(translator.format(
			"modules.groups." + info.group() + ".label", info.group()));
	}

	output.key("order");
	output.value(info.id().toString());

	output.key("state");
	output.startObject();

	if (!control.lastConfirmed().isNull()) {
		output.key("last_confirmed");
		serialize(output, control.lastConfirmed());
	}

	output.key("current");
	serialize(output, control.current());

	output.endObject();

	output.endObject();

}

void BeeeOn::RestUI::serialize(PrintHandler &output,
		const Control::State &state)
{
	output.startObject();

	output.key("stability");
	switch (state.stability()) {
	case Control::State::STABILITY_UNKNOWN:
		output.value(string("unknown"));
		break;
	case Control::State::STABILITY_REQUESTED:
		output.value(string("request"));
		break;
	case Control::State::STABILITY_ACCEPTED:
		output.value(string("accepted"));
		break;
	case Control::State::STABILITY_UNCONFIRMED:
		output.value(string("unconfirmed"));
		break;
	case Control::State::STABILITY_CONFIRMED:
		output.value(string("confirmed"));
		break;
	case Control::State::STABILITY_OVERRIDEN:
		output.value(string("overriden"));
		break;
	case Control::State::STABILITY_STUCK:
		output.value(string("stuck"));
		break;
	case Control::State::STABILITY_FAILED_ROLLBACK:
		output.value(string("failed_rollback"));
		break;
	case Control::State::STABILITY_FAILED_UNKNOWN:
		output.value(string("failed_unknown"));
		break;
	default:
		throw IllegalStateException("unexpected control stability");
	}

	output.key("value");
	valueDouble(output, state.value());

	if (!state.at().isNull()) {
		output.key("at");
		output.value(state.at().value().epochTime());
	}


	switch (state.originatorType()) {
	case Control::State::ORIGINATOR_USER:
		output.key("originator");
		output.startObject();
		output.key("user_id");
		output.value(state.user().id().toString());
		output.endObject();
		break;

	case Control::State::ORIGINATOR_GATEWAY:
		output.key("originator");
		output.startObject();
		output.key("gateway_id");
		output.value(state.gateway().id().toString());
		output.endObject();
		break;

	default: // ignore
		break;
	}

	output.endObject();
}

void BeeeOn::RestUI::serialize(PrintHandler &output,
		Translator &translator,
		const list<Control> &controls)
{
	output.startArray();

	for (auto &control : controls)
		serialize(output, translator, control);

	output.endArray();
}
