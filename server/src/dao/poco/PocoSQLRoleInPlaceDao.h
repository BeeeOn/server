#include "dao/RoleInPlaceDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"
#include "Debug.h"

namespace Poco {
namespace Data {

class Session;
class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLRoleInPlaceDao :
		public PocoAbstractDao,
		public RoleInPlaceDao {
public:
	PocoSQLRoleInPlaceDao();

	void create(RoleInPlace &role) override;
	bool fetch(RoleInPlace &role) override;
	void fetchBy(std::vector<RoleInPlace> &roles,
			const Place &place) override;
	bool hasUsersExcept(const Place &place, const User &user) override;
	bool update(RoleInPlace &role) override;
	bool remove(const RoleInPlace &role) override;

	AccessLevel fetchAccessLevel(
			const Place &place,
			const User &user) override;

	void fetchAccessiblePlaces(
			std::vector<Place> &list,
			const User &user,
			const AccessLevel &atLeast = AccessLevel::any()) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			RoleInPlace &role, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			RoleInPlace &role, const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection)
	{
		for (auto row : result) {
			RoleInPlace role;

			if (!parseSingle(row, role)) {
				LOGGER_FUNC(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(role);
		}
	}

protected:
	void create(Poco::Data::Session &session, RoleInPlace &role);
	bool fetch(Poco::Data::Session &session, RoleInPlace &role);
	void fetchBy(Poco::Data::Session &session,
			std::vector<RoleInPlace> &roles,
			const Place &place);
	bool hasUsersExcept(Poco::Data::Session &session,
			const Place &place,
			const User &user);
	bool update(Poco::Data::Session &session, RoleInPlace &role);
	bool remove(Poco::Data::Session &session, const RoleInPlace &role);

	AccessLevel fetchAccessLevel(Poco::Data::Session &session,
			const Place &place,
			const User &user);

	void fetchAccessiblePlaces(Poco::Data::Session &session,
			std::vector<Place> &list,
			const User &user,
			const AccessLevel &atLeast);

private:
	SQLQuery m_queryCreate           {"roles_in_place.create"};
	SQLQuery m_queryUpdate           {"roles_in_place.update"};
	SQLQuery m_queryRemove           {"roles_in_place.remove"};
	SQLQuery m_queryFetchById        {"roles_in_place.fetch.by.id"};
	SQLQuery m_queryFetchByPlaceId   {"roles_in_place.fetch.by.place_id"};
	SQLQuery m_queryFetchAccessLevel {"roles_in_place.fetch.access_level"};
	SQLQuery m_queryCountUsersExcept {"roles_in_place.count.users.except"};
	SQLQuery m_queryFetchAccessiblePlaces {"roles_in_place.fetch.accessible.places"};
};

}
