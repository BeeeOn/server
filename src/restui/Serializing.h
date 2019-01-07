#pragma once

#include <list>
#include <vector>

#include "provider/TypeInfoProvider.h"
#include "l10n/TimeZone.h"
#include "model/DeviceProperty.h"
#include "model/Control.h"
#include "model/SubtypeInfo.h"
#include "model/TypeInfo.h"

namespace Poco {
namespace JSON {

class PrintHandler;

}

}

namespace BeeeOn {

class Device;
class Gateway;
class GatewayMessage;
class GatewayScan;
class LegacyRoleInGateway;
class Locale;
class Location;
class VerifiedIdentity;
class RoleInGateway;
class Sensor;
class Translator;

namespace RestUI {

void serialize(Poco::JSON::PrintHandler &output,
		const Gateway &gateway,
		const Locale &locale);
void serialize(Poco::JSON::PrintHandler &output,
		const std::vector<Gateway> &gateways,
		const Locale &locale);

void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const GatewayMessage &message);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const std::vector<GatewayMessage> &messages);

void serialize(Poco::JSON::PrintHandler &output,
		const GatewayScan &scan,
		Translator &translator);

void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const VerifiedIdentity &identity);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const std::list<VerifiedIdentity> &identities);

void serialize(Poco::JSON::PrintHandler &output, const Location &location);
void serialize(Poco::JSON::PrintHandler  &output, const std::vector <Location> &locations);

void serialize(Poco::JSON::PrintHandler &output, const RoleInGateway &role);
void serialize(Poco::JSON::PrintHandler &output, const LegacyRoleInGateway &role);
void serialize(Poco::JSON::PrintHandler  &output, const std::vector <RoleInGateway> &roles);

void serialize(Poco::JSON::PrintHandler &output, const Device &device);
void serialize(Poco::JSON::PrintHandler &output,
	       const std::vector<Device> &devices);

void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const DecryptedDeviceProperty &property);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const std::vector<DecryptedDeviceProperty> &properties);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const DevicePropertyKey &key);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const std::vector<DevicePropertyKey> &keys);

void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const TypeInfo &info);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const TypeInfoProvider::InfoSet::const_iterator begin,
		const TypeInfoProvider::InfoSet::const_iterator end);

void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const SubtypeInfo &info);

void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const Sensor &sensor);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const std::list<Sensor> &sensors);

void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const Control &control);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const Control::RequestedValue &requested,
		const ValueAt &recent);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const std::list<Control> &control);

void serialize(Poco::JSON::PrintHandler &output,
		const TimeZone &zone,
		const Locale &locale);
void serialize(Poco::JSON::PrintHandler &output,
		const std::vector<TimeZone> &zones,
		const Locale &locale);

}

}
