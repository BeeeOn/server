#pragma once

#include <string>

#include <Poco/SharedPtr.h>

namespace BeeeOn {
namespace Automation {

/**
 * @brief EventPublisher interface allows to publish information
 * about internal events to some external events processor.
 */
class EventPublisher {
public:
	typedef Poco::SharedPtr<EventPublisher> Ptr;

	virtual ~EventPublisher();

	/**
	 * Publish the event serialized into a suitable format.
	 */
	virtual void publish(const std::string &message) = 0;
};

}
}
