#ifndef BEEEON_RESTUI_SERIALIZING_H
#define BEEEON_RESTUI_SERIALIZING_H

#include <list>
#include <vector>

#include "provider/TypeInfoProvider.h"
#include "l10n/TimeZone.h"
#include "model/Control.h"
#include "model/EnumInfo.h"
#include "model/TypeInfo.h"

namespace Poco {
namespace JSON {

class PrintHandler;

}
}

namespace BeeeOn {

class Device;
class Gateway;
class LegacyRoleInGateway;
class Locale;
class Location;
class VerifiedIdentity;
class RoleInGateway;
class Sensor;
class Translator;
class Work;

namespace RestUI {

void serialize(Poco::JSON::PrintHandler &output,
		const Gateway &gateway,
		const Locale &locale);
void serialize(Poco::JSON::PrintHandler &output,
		const std::vector<Gateway> &gateways,
		const Locale &locale);

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

void serialize(Poco::JSON::PrintHandler &output, const Work &work);
void serialize(Poco::JSON::PrintHandler &output, const std::list<Work> &works);

void serialize(Poco::JSON::PrintHandler &output, const Device &device);
void serialize(Poco::JSON::PrintHandler &output,
	       const std::vector<Device> &devices);

void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const TypeInfo &info);
void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const TypeInfoProvider::InfoSet::const_iterator begin,
		const TypeInfoProvider::InfoSet::const_iterator end);

void serialize(Poco::JSON::PrintHandler &output,
		Translator &translator,
		const EnumInfo &info);

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
		const Control::State &state);
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

#endif
