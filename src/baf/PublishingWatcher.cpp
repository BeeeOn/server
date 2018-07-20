#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "baf/PublishingWatcher.h"

BEEEON_OBJECT_BEGIN(BeeeOn, Automation, PublishingWatcher)
BEEEON_OBJECT_CASTABLE(DeviceListener)
BEEEON_OBJECT_CASTABLE(GatewayListener)
BEEEON_OBJECT_CASTABLE(SensorDataListener)
BEEEON_OBJECT_CASTABLE(ServerListener)
BEEEON_OBJECT_CASTABLE(IdentityListener)
BEEEON_OBJECT_PROPERTY("publishers", &PublishingWatcher::addPublisher)
BEEEON_OBJECT_END(BeeeOn, Automation, PublishingWatcher)

using namespace std;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::Automation;

PublishingWatcher::PublishingWatcher()
{
}

void PublishingWatcher::addPublisher(EventPublisher::Ptr publisher)
{
	m_publishers.emplace_back(publisher);
}

void PublishingWatcher::cleanup()
{
	m_publishers.clear();
}

void PublishingWatcher::onNewDevice(const DeviceEvent &e)
{
	publishEvent(e, "on-new-device");
}

void PublishingWatcher::onRefusedNewDevice(const DeviceEvent &e)
{
	publishEvent(e, "on-refused-new-device");
}

void PublishingWatcher::onPairRequested(const DeviceEvent &e)
{
	publishEvent(e, "on-pair-device", "requested");
}

void PublishingWatcher::onPairConfirmed(const DeviceEvent &e)
{
	publishEvent(e, "on-pair-device", "confirmed");
}

void PublishingWatcher::onPairFailed(const DeviceEvent &e)
{
	publishEvent(e, "on-pair-device", "failed");
}

void PublishingWatcher::onUnpairRequested(const DeviceEvent &e)
{
	publishEvent(e, "on-unpair-device", "requested");
}

void PublishingWatcher::onUnpairConfirmed(const DeviceEvent &e)
{
	publishEvent(e, "on-unpair-device", "confirmed");
}

void PublishingWatcher::onUnpairFailed(const DeviceEvent &e)
{
	publishEvent(e, "on-unpair-device", "failed");
}

void PublishingWatcher::onConnected(const GatewayEvent &e)
{
	publishEvent(e, "on-connected");
}

void PublishingWatcher::onReconnected(const GatewayEvent &e)
{
	publishEvent(e, "on-reconnected");
}

void PublishingWatcher::onDisconnected(const GatewayEvent &e)
{
	publishEvent(e, "on-disconnected");
}

void PublishingWatcher::onReceived(const SensorDataEvent &e)
{
	publishEvent(e, "on-sensor-data");
}

void PublishingWatcher::onUp(const ServerEvent &e)
{
	publishEvent(e, "on-server-up");
}

void PublishingWatcher::onDown(const ServerEvent &e)
{
	publishEvent(e, "on-server-down");
}

void PublishingWatcher::onFirstLogin(const VerifiedIdentityEvent &e)
{
	publishEvent(e, "on-first-login");
}

void PublishingWatcher::onInvite(const IdentityInviteEvent &e)
{
	publishEvent(e, "on-gateway-invite");
}

void PublishingWatcher::eventBegin(
	PrintHandler &json,
	const string &name) const
{
	json.startObject();

	json.key("event");
	json.value(name);
}

void PublishingWatcher::eventEnd(
	PrintHandler &json) const
{
	json.endObject();
}

void PublishingWatcher::eventDetails(
	PrintHandler &json,
	const GatewayEvent &e) const
{
	json.key("gateway_id");
	json.value(e.gatewayID().toString());

	json.key("timestamp");
	json.value(e.stamp().epochMicroseconds());

	json.key("address");
	json.value(e.socketAddress().toString());
}

void PublishingWatcher::eventDetails(
	PrintHandler &json,
	const SensorDataEvent &e) const
{
	json.key("gateway_id");
	json.value(e.gatewayID().toString());

	json.key("device_id");
	json.value(e.deviceID().toString());

	json.key("timestamp");
	json.value(e.stamp().epochMicroseconds());

	json.key("data");
	json.startObject();

	for (const auto &value : e.data()) {
		json.key(value.module().toString());

		if (value.isValid())
			json.value(value.value());
		else
			json.null();
	}

	json.endObject();
}

static void deviceInfoDetails(
	PrintHandler &json,
	const DeviceInfo &info)
{
	json.key("modules");
	json.startArray();

	for (const auto &module : info) {
		json.startObject();

		json.key("controllable");
		json.value(module.isControllable());

		const auto type = module.type();
		if (type.isNull())
			continue;

		json.key("type");
		json.value(type->name());

		if (type->range().isValid()) {
			const auto &range = type->range();

			json.key("range");
			json.startObject();

			if (range.hasMin()) {
				json.key("min");
				json.value(range.min());
			}

			if (range.hasMax()) {
				json.key("max");
				json.value(range.max());
			}

			if (range.hasStep()) {
				json.key("step");
				json.value(range.step());
			}

			json.endObject();
		}

		if (!type->values().empty()) {
			json.key("values");
			json.startObject();

			for (const auto &pair : type->values()) {
				json.key(to_string(pair.first));
				json.value(pair.second);
			}

			json.endObject();
		}

		if (!type->levels().empty()) {
			json.key("levels");
			json.startArray();

			for (const auto &level : type->levels()) {
				json.startObject();

				if (!level.isValid())
					continue;

				if (!level.label().empty()) {
					json.key("label");
					json.value(level.label());
				}

				json.key("attention");
				json.value(TypeInfo::Level::attentionName(level.attention()));

				if (level.min() != level.max()) {
					json.key("min");
					json.value(level.min());
					json.key("max");
					json.value(level.max());
				}
				else {
					json.key("exact");
					json.value(level.min());
				}

				json.endObject();
			}

			json.endArray();
		}

		json.endObject();
	}

	json.endArray();
}

void PublishingWatcher::eventDetails(
	PrintHandler &json,
	const DeviceEvent &e) const
{
	json.key("gateway_id");
	json.value(e.gatewayID().toString());

	json.key("device_id");
	json.value(e.deviceID().toString());

	if (!e.name().empty()) {
		json.key("device_name");
		json.value(e.name());
	}

	if (!e.type().isNull())
		deviceInfoDetails(json, e.type());
}

void PublishingWatcher::eventDetails(
	PrintHandler &json,
	const DeviceEvent &e,
	const string &status) const
{
	json.key("gateway_id");
	json.value(e.gatewayID().toString());

	json.key("device_id");
	json.value(e.deviceID().toString());

	if (!e.name().empty()) {
		json.key("device_name");
		json.value(e.name());
	}

	json.key("status");
	json.value(status);
}

void PublishingWatcher::eventDetails(
	PrintHandler &json,
	const ServerEvent &e) const
{
	json.key("bind");
	json.value(e.bind());

	json.key("label");
	json.value(e.label());
}

void PublishingWatcher::eventDetails(
	PrintHandler &json,
	const VerifiedIdentityEvent &e) const
{
	const auto &verified = e.verifiedIdentity();

	json.key("identity");
	json.startObject();

	json.key("email");
	json.value(verified.email());

	json.key("id");
	json.value(verified.identity().id().toString());

	json.endObject();

	json.key("provider");
	json.value(verified.provider());

	json.key("user_id");
	json.value(verified.user().id().toString());
}

void PublishingWatcher::eventDetails(
	PrintHandler &json,
	const IdentityInviteEvent &e) const
{
	json.key("originator_id");
	json.value(e.originator().id().toString());

	json.key("gateway_id");
	json.value(e.gateway().id().toString());

	json.key("identity");
	json.startObject();

	json.key("email");
	json.value(e.identity().email());

	json.key("id");
	json.value(e.identity().id().toString());

	json.endObject();

	json.key("access_level");
	json.startObject();

	json.key("group");
	json.value(e.level().toString());

	json.key("value");
	json.value(static_cast<unsigned int>(e.level()));

	json.endObject();
}

void PublishingWatcher::publish(const string &message)
{
	for (auto publisher : m_publishers) {
		try {
			publisher->publish(message);
		}
		BEEEON_CATCH_CHAIN(logger())
	}
}
