#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/Device.h"
#include "model/DeviceProperty.h"
#include "model/Gateway.h"
#include "model/User.h"
#include "service/DeviceService.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/DevicePropertiesRestHandler.h"
#include "restui/JSONDevicePropertyDeserializer.h"
#include "restui/Serializing.h"
#include "util/JsonUtil.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, DevicePropertiesRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_PROPERTY("deviceService", &DevicePropertiesRestHandler::setDeviceService)
BEEEON_OBJECT_PROPERTY("cryptoConfig", &DevicePropertiesRestHandler::setCryptoConfig)
BEEEON_OBJECT_END(BeeeOn, RestUI, DevicePropertiesRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

DevicePropertiesRestHandler::DevicePropertiesRestHandler():
		JSONRestHandler("device_properties")
{
	registerAction<DevicePropertiesRestHandler>(
		"keys",
		&DevicePropertiesRestHandler::keys
	);
	registerAction<DevicePropertiesRestHandler>(
		"list",
		&DevicePropertiesRestHandler::list,
		{"gateway_id", "device_id"}
	);
	registerAction<DevicePropertiesRestHandler>(
		"detail",
		&DevicePropertiesRestHandler::detail,
		{"gateway_id", "device_id", "key"}
	);
	registerAction<DevicePropertiesRestHandler>(
		"create",
		&DevicePropertiesRestHandler::create,
		{"gateway_id", "device_id", "key"}
	);
	registerAction<DevicePropertiesRestHandler>(
		"update",
		&DevicePropertiesRestHandler::update,
		{"gateway_id", "device_id", "key"}
	);
	registerAction<DevicePropertiesRestHandler>(
		"remove",
		&DevicePropertiesRestHandler::remove,
		{"gateway_id", "device_id", "key"}
	);
}

void DevicePropertiesRestHandler::setDeviceService(DeviceService::Ptr service)
{
	m_service = service;
}

void DevicePropertiesRestHandler::setCryptoConfig(SharedPtr<CryptoConfig> config)
{
	m_cryptoConfig = config;
}

void DevicePropertiesRestHandler::keys(RestFlow &flow)
{
	vector<DevicePropertyKey> keys;
	for (const auto &key : DevicePropertyKey::all()) {
		if (key.isUserReadable() || key.isUserWritable())
			keys.emplace_back(key);
	}

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *flow.translator(), keys);
	endSuccess(result);
}

void DevicePropertiesRestHandler::list(RestFlow &flow)
{
	User user(flow.session()->userID());
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway({GatewayID::parse(flow.param("gateway_id"))});

	std::list<DeviceProperty> properties;
	Relation<std::list<DeviceProperty>, Device> input(properties, device);
	input.setUser(user);

	m_service->listProperties(input);

	vector<DecryptedDeviceProperty> decrypted;
	for (const auto &property : properties)
		decrypted.emplace_back(DecryptedDeviceProperty(property, m_cryptoConfig));

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *flow.translator(), decrypted);
	endSuccess(result);
}

void DevicePropertiesRestHandler::detail(RestFlow &flow)
{
	User user(flow.session()->userID());
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway({GatewayID::parse(flow.param("gateway_id"))});

	DeviceProperty property;
	property.setKey(DevicePropertyKey::parse(flow.param("key")));
	Relation<DeviceProperty, Device> input(property, device);
	input.setUser(user);

	if (!m_service->findProperty(input))
		throw NotFoundException("no such device property");

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *flow.translator(),
		DecryptedDeviceProperty(property, m_cryptoConfig));
	endSuccess(result);
}

void DevicePropertiesRestHandler::create(RestFlow &flow)
{
	User user(flow.session()->userID());
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway({GatewayID::parse(flow.param("gateway_id"))});

	DeviceProperty property;
	property.setKey(DevicePropertyKey::parse(flow.param("key")));

	if (!property.key().isUserWritable()) {
		flow.response().setStatus(405);
		PrintHandler result(flow.response().stream());
		beginError(result, 405, "Method Not Allowed");
		result.value(flow.translator()->formatSure("entity_not_writable"));
		endError(result);
		return;
	}

	const Object::Ptr object = parseInput(flow);
	JSONDevicePropertyDeserializer deserializer(object, m_cryptoConfig);
	RelationWithData<DeviceProperty, Device> input(property, deserializer, device);
	input.setUser(user);

	if (!m_service->createProperty(input)) {
		flow.response().setStatus(409);
		PrintHandler result(flow.response().stream());
		beginError(result, 409, "Conflict");
		result.value(flow.translator()->formatSure("already_exists"));
		endError(result);
	}
	else {
		PrintHandler result(flow.response().stream());
		beginSuccess(result, 200);
		serialize(result, *flow.translator(),
			DecryptedDeviceProperty(property, m_cryptoConfig));
		endSuccess(result);
	}
}

void DevicePropertiesRestHandler::update(RestFlow &flow)
{
	User user(flow.session()->userID());
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway({GatewayID::parse(flow.param("gateway_id"))});

	DeviceProperty property;
	property.setKey(DevicePropertyKey::parse(flow.param("key")));

	if (!property.key().isUserWritable()) {
		flow.response().setStatus(405);
		PrintHandler result(flow.response().stream());
		beginError(result, 405, "Method Not Allowed");
		result.value(flow.translator()->formatSure("entity_not_writable"));
		endError(result);
		return;
	}

	const Object::Ptr object = parseInput(flow);
	JSONDevicePropertyDeserializer deserializer(object, m_cryptoConfig);
	RelationWithData<DeviceProperty, Device> input(property, deserializer, device);
	input.setUser(user);

	if (!m_service->updateProperty(input))
		throw NotFoundException("no such property");

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *flow.translator(),
		DecryptedDeviceProperty(property, m_cryptoConfig));
	endSuccess(result);
}

void DevicePropertiesRestHandler::remove(RestFlow &flow)
{
	User user(flow.session()->userID());
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway({GatewayID::parse(flow.param("gateway_id"))});

	DeviceProperty property;
	property.setKey(DevicePropertyKey::parse(flow.param("key")));

	if (!property.key().isUserWritable()) {
		flow.response().setStatus(405);
		PrintHandler result(flow.response().stream());
		beginError(result, 405, "Method Not Allowed");
		result.value(flow.translator()->formatSure("entity_not_writable"));
		endError(result);
		return;
	}

	Relation<const DeviceProperty, Device> input(property, device);
	input.setUser(user);

	if (!m_service->removeProperty(input))
		throw NotFoundException("no such property");

	flow.response().setStatus(204);
	flow.response()["Location"] = flow.linker().link(
		"device_properties", "list",
		{device.gateway().id().toString(), device.id().toString()});
}
