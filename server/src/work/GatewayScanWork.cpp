#include <string>

#include "work/GatewayScanWork.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

GatewayScanWork::GatewayScanWork():
	AbstractGatewayWork(ClassInfo::forPointer(this))
{
}

GatewayScanWork::GatewayScanWork(const WorkContent &content):
	AbstractGatewayWork(content)
{
	if (!type().is<GatewayScanWork>())
		throw InvalidArgumentException("incompatible work content");
}

GatewayScanWork::GatewayScanWork(const Object::Ptr json):
	AbstractGatewayWork(json)
{
	if (!type().is<GatewayScanWork>())
		throw InvalidArgumentException("incompatible work content");
}

void GatewayScanWork::setGatewayID(const GatewayID &id)
{
	json()->set("gateway_id", id.toString());
}

GatewayID GatewayScanWork::gatewayID() const
{
	return GatewayID::parse(json()->getValue<string>("gateway_id"));
}

void GatewayScanWork::setDuration(const Timespan &duration)
{
	json()->set("duration", duration.totalSeconds());
}

Timespan GatewayScanWork::duration() const
{
	return Timespan(json()->getValue<unsigned long>("duration")
		* Timespan::SECONDS);
}

void GatewayScanWork::setStarted(const Timestamp &time)
{
	json()->set("started", time.epochTime());
}

Timestamp GatewayScanWork::started() const
{
	return Timestamp::fromEpochTime(json()->getValue<time_t>("started"));
}
