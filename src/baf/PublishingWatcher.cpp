#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "baf/PublishingWatcher.h"

BEEEON_OBJECT_BEGIN(BeeeOn, Automation, PublishingWatcher)
BEEEON_OBJECT_CASTABLE(DeviceListener)
BEEEON_OBJECT_CASTABLE(GatewayListener)
BEEEON_OBJECT_CASTABLE(SensorDataListener)
BEEEON_OBJECT_CASTABLE(ServerListener)
BEEEON_OBJECT_PROPERTY("publishers", &PublishingWatcher::addPublisher)
BEEEON_OBJECT_END(BeeeOn, Automation, PublishingWatcher)

using namespace std;
using namespace Poco::JSON;
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

void PublishingWatcher::publish(const string &message)
{
	for (auto publisher : m_publishers) {
		try {
			publisher->publish(message);
		}
		BEEEON_CATCH_CHAIN(logger())
	}
}
