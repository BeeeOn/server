#include "dao/IdentityDao.h"
#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class Session;

}
}

namespace BeeeOn {

class PocoSQLIdentityDao :
		public PocoAbstractDao,
		public IdentityDao {
public:
	void create(Identity &identity) override;
	bool fetch(Identity &identity) override;
	bool fetchBy(Identity &identity,
			const std::string &email) override;
	bool remove(const Identity &identity) override;

protected:
	void create(Poco::Data::Session &session, Identity &identity);
	bool fetch(Poco::Data::Session &session, Identity &identity);
	bool fetchBy(Poco::Data::Session &session, Identity &identity,
			const std::string &email);
	bool remove(Poco::Data::Session &session, const Identity &identity);
};

}
