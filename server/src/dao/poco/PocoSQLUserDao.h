#include "dao/UserDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLUserDao :
		public PocoAbstractDao,
		public UserDao {
public:
	PocoSQLUserDao();

	void create(User &user) override;
	bool fetch(User &user) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			User &user, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			User &user, const std::string &prefix = "");
	static bool parseIfIDNotNull(Poco::Data::Row &result,
			User &user, const std::string &prefix = "");

private:
	SQLQuery m_queryCreate {"users.create"};
	SQLQuery m_queryFetch  {"users.fetch.by.id"};
};

}
