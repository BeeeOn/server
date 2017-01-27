#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RowIterator.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RecordSet.h>

#include "dao/poco/PocoSQLRoleInGatewayDao.h"
#include "dao/poco/PocoSQLGatewayDao.h"
#include "dao/poco/PocoSQLIdentityDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "model/User.h"
#include "model/Gateway.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLRoleInGatewayDao, BeeeOn::PocoSQLRoleInGatewayDao)

PocoSQLRoleInGatewayDao::PocoSQLRoleInGatewayDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryRemove);
	registerQuery(m_queryRemoveUser);
	registerQuery(m_queryRemoveAll);
	registerQuery(m_queryIsUser);
	registerQuery(m_queryIsRegistered);
	registerQuery(m_queryFetchById);
	registerQuery(m_queryFetchByGatewayId);
	registerQuery(m_queryFetchAccessLevel);
	registerQuery(m_queryFetchAccessibleGateways);
	registerQuery(m_queryHasOnlyNonAdminExcept);
}

void PocoSQLRoleInGatewayDao::create(RoleInGateway &role)
{
	assureHasId(role.gateway());
	assureHasId(role.identity());

	role.setId(RoleInGatewayID::random());
	string id(role.id().toString());
	string gatewayID(role.gateway().id().toString());
	string identityID(role.identity().id().toString());
	unsigned int level = role.level();
	unsigned long created = role.created().timestamp().epochTime();

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(gatewayID, "gateway_id"),
		use(identityID, "identity_id"),
		use(level, "level"),
		use(created, "created")
	);

	execute(sql);
}

bool PocoSQLRoleInGatewayDao::fetch(RoleInGateway &role)
{
	assureHasId(role);

	string roleID(role.id().toString());

	Statement sql = (session() << m_queryFetchById(),
		use(roleID, "role_id")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, role);
}

void PocoSQLRoleInGatewayDao::fetchBy(std::vector<RoleInGateway> &roles,
		const Gateway &gateway)
{
	assureHasId(gateway);

	string gatewayID(gateway.id().toString());

	Statement sql = (session() << m_queryFetchByGatewayId(),
		use(gatewayID, "gateway_id")
	);

	execute(sql);
	RecordSet result(sql);
	parseMany(result, roles);
}

bool PocoSQLRoleInGatewayDao::remove(const RoleInGateway &role)
{
	assureHasId(role);

	string id(role.id().toString());

	Statement sql = (session() << m_queryRemove(),
		use(id, "id")
	);

	return execute(sql) > 0;
}

bool PocoSQLRoleInGatewayDao::remove(
		const Gateway &gateway, const User &user)
{
	assureHasId(gateway);
	assureHasId(user);

	string gatewayID(gateway.id().toString());
	string userID(user.id().toString());

	Statement sql = (session() << m_queryRemoveUser(),
		use(userID, "user_id"),
		use(gatewayID, "gateway_id")
	);

	return execute(sql) > 0;
}

void PocoSQLRoleInGatewayDao::removeAll(const Gateway &gateway)
{
	assureHasId(gateway);

	string gatewayID(gateway.id().toString());

	Statement sql = (session() << m_queryRemoveAll(),
		use(gatewayID, "gateway_id")
	);

	execute(sql);
}

bool PocoSQLRoleInGatewayDao::isUser(const RoleInGateway &role,
		const User &user)
{
	assureHasId(role);
	assureHasId(user);

	string roleID(role.id().toString());
	string userID(user.id().toString());
	bool result = false;

	Statement sql = (session() << m_queryIsUser(),
		use(roleID, "role_id"),
		use(userID, "user_id"),
		into(result)
	);

	execute(sql);
	return result;
}

bool PocoSQLRoleInGatewayDao::isRegistered(
		const Gateway &gateway)
{
	assureHasId(gateway);

	string gatewayID(gateway.id().toString());
	bool result = false;

	Statement sql = (session() << m_queryIsRegistered(),
		use(gatewayID, "gateway_id"),
		into(result)
	);

	execute(sql);
	return result;
}

bool PocoSQLRoleInGatewayDao::hasOnlyNonAdminExcept(
		const Gateway &gateway, const User &user)
{
	assureHasId(gateway);
	assureHasId(user);

	string gatewayID(gateway.id().toString());
	string userID(user.id().toString());
	unsigned int level = AccessLevel::admin();
	bool result = false;

	Statement sql = (session() << m_queryHasOnlyNonAdminExcept(),
		use(level, "level"),
		use(gatewayID, "gateway_id"),
		use(userID, "user_id"),
		into(result)
	);

	execute(sql);
	return result;
}

AccessLevel PocoSQLRoleInGatewayDao::fetchAccessLevel(
		const Gateway &gateway,
		const User &user)
{
	assureHasId(gateway);
	assureHasId(user);

	string gatewayID(gateway.id().toString());
	string userID(user.id().toString());
	Nullable<unsigned int> level;

	Statement sql = (session() << m_queryFetchAccessLevel(),
		use(gatewayID, "gateway_id"),
		use(userID, "user_id"),
		into(level)
	);

	execute(sql);
	return level.isNull()? AccessLevel::none() : AccessLevel(level);
}

void PocoSQLRoleInGatewayDao::fetchAccessibleGateways(
		std::vector<Gateway> &list,
		const User &user,
		const AccessLevel &atLeast)
{
	assureHasId(user);

	string userID(user.id().toString());
	unsigned int level = atLeast;

	Statement sql = (session() << m_queryFetchAccessibleGateways(),
		use(level, "at_least"),
		use(userID, "user_id")
	);

	execute(sql);
	RecordSet result(sql);
	PocoSQLGatewayDao::parseMany<Gateway>(result, list);
}

bool PocoSQLRoleInGatewayDao::parseSingle(RecordSet &result,
		RoleInGateway &role, const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), role, prefix);
}

bool PocoSQLRoleInGatewayDao::parseSingle(Row &result,
		RoleInGateway &role, const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		role.setId(RoleInGatewayID::parse(result[prefix + "id"]));

	role.setLevel(AccessLevel(result[prefix + "level"].convert<unsigned int>()));
	role.setCreated(Timestamp::fromEpochTime(result[prefix + "created"]));

	Gateway gateway(GatewayID::parse(result[prefix + "gateway_id"]));
	role.setGateway(gateway);

	Identity identity;
	if (!PocoSQLIdentityDao::parseIfIDNotNull(result, identity, prefix + "identity_"))
		throw IllegalStateException("identity is incomplete in query request");

	role.setIdentity(identity);

	markLoaded(role);
	return true;
}
