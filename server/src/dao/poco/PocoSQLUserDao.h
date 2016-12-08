#include "dao/UserDao.h"
#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class Session;

}
}

namespace BeeeOn {

class PocoSQLUserDao :
		public PocoAbstractDao,
		public UserDao {
public:
	void create(User &user) override;
	bool fetch(User &user) override;

protected:
	void create(Poco::Data::Session &session, User &user);
	bool fetch(Poco::Data::Session &session, User &user);
};

}
