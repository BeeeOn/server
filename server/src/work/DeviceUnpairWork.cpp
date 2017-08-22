#include <Poco/Exception.h>

#include "util/JsonUtil.h"
#include "work/DeviceUnpairWork.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

DeviceUnpairWork::DeviceUnpairWork():
	WorkContent(ClassInfo::forPointer(this))
{
	json()->set("attempt", 0);
}

DeviceUnpairWork::DeviceUnpairWork(const WorkContent &content):
	WorkContent(content)
{
	if (!type().is<DeviceUnpairWork>())
		throw InvalidArgumentException("uncompatible work content");
}

DeviceUnpairWork::DeviceUnpairWork(const Object::Ptr json):
	WorkContent(json)
{
	if (!type().is<DeviceUnpairWork>())
		throw InvalidArgumentException("uncompatible work content");
}

void DeviceUnpairWork::setAttempt(unsigned int attempt)
{
	json()->set("attempt", attempt);
}

unsigned int DeviceUnpairWork::attempt() const
{
	return JsonUtil::extract<unsigned int>(json(), "attempt");
}

void DeviceUnpairWork::setGatewayID(const GatewayID &id)
{
	json()->set("gateway_id", id.toString());
}

GatewayID DeviceUnpairWork::gatewayID() const
{
	return GatewayID::parse(
		JsonUtil::extract<string>(json(), "gateway_id")
	);
}

void DeviceUnpairWork::setDeviceID(const DeviceID &id)
{
	json()->set("device_id", id.toString());
}

DeviceID DeviceUnpairWork::deviceID() const
{
	return DeviceID::parse(
		JsonUtil::extract<string>(json(), "device_id")
	);
}
