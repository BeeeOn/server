#include <Poco/Exception.h>

#include "util/JsonUtil.h"
#include "work/DeviceUnpairWork.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

DeviceUnpairWork::DeviceUnpairWork():
	AbstractGatewayWork(ClassInfo::forPointer(this))
{
}

DeviceUnpairWork::DeviceUnpairWork(const WorkContent &content):
	AbstractGatewayWork(content)
{
	if (!type().is<DeviceUnpairWork>())
		throw InvalidArgumentException("incompatible work content");
}

DeviceUnpairWork::DeviceUnpairWork(const Object::Ptr json):
	AbstractGatewayWork(json)
{
	if (!type().is<DeviceUnpairWork>())
		throw InvalidArgumentException("incompatible work content");
}

void DeviceUnpairWork::setGatewayID(const GatewayID &id)
{
	json()->set("gateway_id", id.toString());
}

GatewayID DeviceUnpairWork::gatewayID() const
{
	return GatewayID::parse(
		json()->getValue<string>("gateway_id")
	);
}

void DeviceUnpairWork::setDeviceID(const DeviceID &id)
{
	json()->set("device_id", id.toString());
}

DeviceID DeviceUnpairWork::deviceID() const
{
	return DeviceID::parse(
		json()->getValue<string>("device_id")
	);
}
