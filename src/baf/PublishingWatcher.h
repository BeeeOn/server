#pragma once

#include <list>
#include <sstream>
#include <string>

#include <Poco/JSON/PrintHandler.h>

#include "baf/EventPublisher.h"
#include "gws/DeviceListener.h"
#include "gws/GatewayListener.h"
#include "gws/SensorDataListener.h"
#include "server/ServerListener.h"
#include "util/Loggable.h"

namespace BeeeOn {
namespace Automation {

class PublishingWatcher :
	public DeviceListener,
	public GatewayListener,
	public SensorDataListener,
	public ServerListener,
	Loggable {
public:
	PublishingWatcher();

	void addPublisher(EventPublisher::Ptr publisher);

	void onNewDevice(const DeviceEvent &e) override;
	void onRefusedNewDevice(const DeviceEvent &e) override;

	void onConnected(const GatewayEvent &e) override;
	void onReconnected(const GatewayEvent &e) override;
	void onDisconnected(const GatewayEvent &e) override;

	void onReceived(const SensorDataEvent &e) override;

	void onUp(const ServerEvent &e) override;
	void onDown(const ServerEvent &e) override;

	void cleanup();

protected:
	void eventBegin(
		Poco::JSON::PrintHandler &json,
		const std::string &name) const;
	void eventEnd(
		Poco::JSON::PrintHandler &json) const;
	void eventDetails(
		Poco::JSON::PrintHandler &json,
		const GatewayEvent &e) const;
	void eventDetails(
		Poco::JSON::PrintHandler &json,
		const SensorDataEvent &e) const;
	void eventDetails(
		Poco::JSON::PrintHandler &json,
		const DeviceEvent &e) const;
	void eventDetails(
		Poco::JSON::PrintHandler &json,
		const DeviceEvent &e,
		const std::string &status) const;
	void eventDetails(
		Poco::JSON::PrintHandler &json,
		const ServerEvent &e) const;

	template <typename Event>
	void publishEvent(const Event &e, const std::string &name)
	{
		if (m_publishers.empty())
			return;

		std::ostringstream buf;
		Poco::JSON::PrintHandler json(buf);

		eventBegin(json, name);
		eventDetails(json, e);
		eventEnd(json);

		publish(buf.str());
	}

	template <typename Event, typename Status>
	void publishEvent(const Event &e, const std::string &name, const Status &status)
	{
		if (m_publishers.empty())
			return;

		std::ostringstream buf;
		Poco::JSON::PrintHandler json(buf);

		eventBegin(json, name);
		eventDetails(json, e, status);
		eventEnd(json);

		publish(buf.str());
	}

	void publish(const std::string &message);

private:
	std::list<EventPublisher::Ptr> m_publishers;
};

}
}
