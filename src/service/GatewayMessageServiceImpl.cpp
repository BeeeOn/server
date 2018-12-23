#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "service/GatewayMessageServiceImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GatewayMessageServiceImpl)
BEEEON_OBJECT_CASTABLE(GatewayMessageService)
BEEEON_OBJECT_CASTABLE(DeviceListener)
BEEEON_OBJECT_PROPERTY("gatewayMessageDao", &GatewayMessageServiceImpl::setGatewayMessageDao)
BEEEON_OBJECT_PROPERTY("accessPolicy", &GatewayMessageServiceImpl::setAccessPolicy)
BEEEON_OBJECT_PROPERTY("transactionManager", &GatewayMessageServiceImpl::setTransactionManager)
BEEEON_OBJECT_PROPERTY("gatewayMessagesLimit", &GatewayMessageServiceImpl::setGatewayMessagesLimit)
BEEEON_OBJECT_END(BeeeOn, GatewayMessageServiceImpl)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

GatewayMessageServiceImpl::GatewayMessageServiceImpl()
{
}

void GatewayMessageServiceImpl::setAccessPolicy(GatewayMessageAccessPolicy::Ptr policy)
{
	m_policy = policy;
}

void GatewayMessageServiceImpl::setGatewayMessageDao(GatewayMessageDao::Ptr dao)
{
	m_dao = dao;
}

void GatewayMessageServiceImpl::setGatewayMessagesLimit(int limit)
{
	if (limit < 1)
		throw InvalidArgumentException("gatewayMessagesLimit must be at least 1");

	m_gatewayMessagesLimit = limit;
}

bool GatewayMessageServiceImpl::doFetch(
		Relation<GatewayMessage, Gateway> &input)
{
	m_policy->assure(
		GatewayMessageAccessPolicy::ACTION_USER_GET,
		input,
		input.base());

	return m_dao->fetch(input.target(), input.base());
}

void GatewayMessageServiceImpl::doFetchMany(
		Relation<vector<GatewayMessage>, Gateway> &input)
{
	m_policy->assure(
		GatewayMessageAccessPolicy::ACTION_USER_GET,
		input,
		input.base());

	m_dao->fetchBy(input.target(), input.base());
}

bool GatewayMessageServiceImpl::doRemove(
		Relation<GatewayMessage, Gateway> &input)
{
	m_policy->assure(
		GatewayMessageAccessPolicy::ACTION_USER_REMOVE,
		input,
		input.base());

	return m_dao->remove(input.target(), input.base());
}

void GatewayMessageServiceImpl::onRefusedNewDevice(const DeviceEvent &e)
{
	GatewayMessage message;
	message.setGateway({e.gatewayID()});
	message.setAt({});
	message.setSeverity(GatewayMessage::SEVERITY_WARN);
	message.setKey("gateway.new_device_refused");

	Object::Ptr context = new Object;
	context->set("gateway_id", e.gatewayID().toString());
	context->set("device_id", e.deviceID().toString());

	message.setContext(context);

	BEEEON_TRANSACTION(m_dao->insert(message, m_gatewayMessagesLimit));
}

void GatewayMessageServiceImpl::onPairFailed(const DeviceEvent &e)
{
	GatewayMessage message;
	message.setGateway({e.gatewayID()});
	message.setAt({});
	message.setSeverity(GatewayMessage::SEVERITY_ERROR);
	message.setKey("gateway.pair_device_failed");

	Object::Ptr context = new Object;
	context->set("gateway_id", e.gatewayID().toString());
	context->set("device_id", e.deviceID().toString());

	message.setContext(context);

	BEEEON_TRANSACTION(m_dao->insert(message, m_gatewayMessagesLimit));
}

void GatewayMessageServiceImpl::onUnpairFailed(const DeviceEvent &e)
{
	GatewayMessage message;
	message.setGateway({e.gatewayID()});
	message.setAt({});
	message.setSeverity(GatewayMessage::SEVERITY_ERROR);
	message.setKey("gateway.unpair_device_failed");

	Object::Ptr context = new Object;
	context->set("gateway_id", e.gatewayID().toString());
	context->set("device_id", e.deviceID().toString());

	message.setContext(context);

	BEEEON_TRANSACTION(m_dao->insert(message, m_gatewayMessagesLimit));
}
