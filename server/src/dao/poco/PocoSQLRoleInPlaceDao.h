#include "dao/RoleInPlaceDao.h"
#include "dao/poco/PocoAbstractDao.h"

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
};

}
