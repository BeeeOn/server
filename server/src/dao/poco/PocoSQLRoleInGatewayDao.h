#include "dao/RoleInGatewayDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"
#include "Debug.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLRoleInGatewayDao :
		public PocoAbstractDao,
		public RoleInGatewayDao {
public:
	PocoSQLRoleInGatewayDao();

	void create(RoleInGateway &role) override;
	void fetchBy(std::vector<RoleInGateway> &roles,
			const Gateway &gateway) override;
	bool remove(const RoleInGateway &role) override;
	bool remove(const Gateway &gateway,
			const User &user) override;

	bool isRegistered(const Gateway &gateway) override;

	AccessLevel fetchAccessLevel(
			const Gateway &gateway,
			const User &user) override;

	void fetchAccessibleGateways(
			std::vector<Gateway> &list,
			const User &user,
			const AccessLevel &atLeast = AccessLevel::any()) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			RoleInGateway &role, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			RoleInGateway &role, const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection)
	{
		for (auto row : result) {
			RoleInGateway role;

			if (!parseSingle(row, role)) {
				LOGGER_FUNC(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(role);
		}
	}

private:
	SQLQuery m_queryCreate           {"roles_in_gateway.create"};
	SQLQuery m_queryRemove           {"roles_in_gateway.remove"};
	SQLQuery m_queryRemoveUser       {"roles_in_gateway.remove.user"};
	SQLQuery m_queryIsRegistered     {"roles_in_gateway.is.registered"};
	SQLQuery m_queryFetchByGatewayId   {"roles_in_gateway.fetch.by.gateway_id"};
	SQLQuery m_queryFetchAccessLevel {"roles_in_gateway.fetch.access_level"};
	SQLQuery m_queryFetchAccessibleGateways {"roles_in_gateway.fetch.accessible.gateways"};
};

}
