#pragma once

#include <list>
#include <sstream>
#include <string>

#include <Poco/JSON/PrintHandler.h>

#include "baf/EventPublisher.h"
#include "gws/GatewayListener.h"
#include "gws/SensorDataListener.h"
#include "util/Loggable.h"

namespace BeeeOn {
namespace Automation {

class PublishingWatcher :
	public GatewayListener,
	public SensorDataListener,
	Loggable {
public:
	PublishingWatcher();

	void addPublisher(EventPublisher::Ptr publisher);

	void onConnected(const GatewayEvent &e) override;
	void onReconnected(const GatewayEvent &e) override;
	void onDisconnected(const GatewayEvent &e) override;

	void onReceived(const SensorDataEvent &e) override;

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

	void publish(const std::string &message);

private:
	std::list<EventPublisher::Ptr> m_publishers;
};

}
}
