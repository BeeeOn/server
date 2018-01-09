#include "dao/RoleInGatewayDao.h"
#include "dao/Query.h"
#include "dao/poco/PocoAbstractDao.h"
#include "l10n/TimeZoneProvider.h"
#include "util/Loggable.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class Identity;
class VerifiedIdentity;

class PocoSQLRoleInGatewayDao :
		public PocoAbstractDao,
		public RoleInGatewayDao {
public:
	PocoSQLRoleInGatewayDao();

	void setTimeZoneProvider(TimeZoneProvider::Ptr provider);

	void create(RoleInGateway &role) override;
	bool update(RoleInGateway &role) override;
	bool fetch(RoleInGateway &role) override;
	bool fetch(LegacyRoleInGateway &role) override;
	void fetchBy(std::vector<RoleInGateway> &roles,
			const Gateway &gateway) override;
	void fetchBy(std::vector<LegacyRoleInGateway> &roles,
			const Gateway &gateway) override;
	bool remove(const RoleInGateway &role) override;
	bool remove(const Gateway &gateway,
			const User &user) override;
	void removeAll(const Gateway &gateway) override;

	bool isUser(const RoleInGateway &role, const User &user) override;
	bool isRegistered(const Gateway &gateway) override;
	bool hasOnlyNonAdminExcept(
			const Gateway &gateway,
			const User &user) override;

	AccessLevel fetchAccessLevel(
			const Gateway &gateway,
			const User &user) override;

	void fetchAccessibleGateways(
			std::vector<Gateway> &list,
			const User &user,
			const AccessLevel &atLeast = AccessLevel::any()) override;

	bool canSeeIdentity(
		const Identity &identity,
		const User &user) override;
	bool canSeeVerifiedIdentity(
		const VerifiedIdentity &identity,
		const User &user) override;

	template <typename R>
	static bool parseSingle(Poco::Data::RecordSet &result,
			R &role, const std::string &prefix = "")
	{
		if (result.begin() == result.end())
			return false;

		return parseSingle(*result.begin(), role, prefix);
	}

	static bool parseSingle(Poco::Data::Row &result,
			RoleInGateway &role, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			LegacyRoleInGateway &role, const std::string &prefix = "");

	template <typename R, typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection)
	{
		for (auto row : result) {
			R role;

			if (!parseSingle(row, role)) {
				Loggable::forMethod(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(role);
		}
	}

private:
	Query m_queryCreate           {"roles_in_gateway.create"};
	Query m_queryUpdate           {"roles_in_gateway.update"};
	Query m_queryRemove           {"roles_in_gateway.remove"};
	Query m_queryRemoveUser       {"roles_in_gateway.remove.user"};
	Query m_queryRemoveAll        {"roles_in_gateway.remove.all"};
	Query m_queryIsUser           {"roles_in_gateway.is.user"};
	Query m_queryIsRegistered     {"roles_in_gateway.is.registered"};
	Query m_queryFetchById        {"roles_in_gateway.fetch.by.id"};
	Query m_queryFetchByGatewayId   {"roles_in_gateway.fetch.by.gateway_id"};
	Query m_queryFetchLegacyByGatewayId {"legacy_roles_in_gateway.fetch.by.gateway_id"};
	Query m_queryFetchLegacyById  {"legacy_roles_in_gateway.fetch.by.id"};
	Query m_queryFetchAccessLevel {"roles_in_gateway.fetch.access_level"};
	Query m_queryFetchAccessibleGateways {"roles_in_gateway.fetch.accessible.gateways"};
	Query m_queryHasOnlyNonAdminExcept {"roles_in_gateway.has.only.given.level.except"};
	Query m_queryCanSeeIdentity   {"roles_in_gateway.can.see.identity"};
	Query m_queryCanSeeVerifiedIdentity {"roles_in_gateway.can.see.verified_identity"};

	TimeZoneProvider::Ptr m_timeZoneProvider;
};

}
