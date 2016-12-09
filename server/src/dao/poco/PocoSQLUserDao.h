#include "dao/UserDao.h"
#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class Session;
class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLUserDao :
		public PocoAbstractDao,
		public UserDao {
public:
	void create(User &user) override;
	bool fetch(User &user) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			User &user, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			User &user, const std::string &prefix = "");
	static bool parseIfIDNotNull(Poco::Data::Row &result,
			User &user, const std::string &prefix = "");

protected:
	void create(Poco::Data::Session &session, User &user);
	bool fetch(Poco::Data::Session &session, User &user);
};

}
