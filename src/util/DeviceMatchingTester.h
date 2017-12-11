#pragma once

#include <Poco/Activity.h>
#include <Poco/SharedPtr.h>

#include "io/Console.h"
#include "io/Printable.h"
#include "loop/Tool.h"
#include "model/DeviceInfo.h"
#include "provider/DeviceInfoProvider.h"

namespace BeeeOn {

/**
 * @brief Utility for interactive testing of matching of devices agains the current
 * devices database (usually from devices.xml). It is intended to run the DeviceMatchingTester
 * as an alternative initial instance of the application. It is designed to work
 * with the BeeeOn::StdConsole for the interactive loop.
 */
class DeviceMatchingTester : public Tool {
public:
	DeviceMatchingTester();
	~DeviceMatchingTester();

	void setDeviceProvider(DeviceInfoProvider::Ptr provider);

protected:
	void main(
		ConsoleSession &session,
		const std::vector<std::string> &args) override;

	bool processLine(const std::string &line, Printable &out);
	void reportShort(const Poco::SharedPtr<DeviceInfo> info, Printable &out);
	void reportResult(const Poco::SharedPtr<DeviceInfo> info, Printable &out);

private:
	DeviceInfoProvider::Ptr m_provider;
};

}
