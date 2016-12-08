#include "dao/IdentityDao.h"
#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class Session;
class RecordSet;
class Row;

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

	static bool parseSingle(Poco::Data::RecordSet &result,
			Identity &identity, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			Identity &identity, const std::string &prefix = "");
	static bool parseIfIDNotNull(Poco::Data::Row &result,
			Identity &identity, const std::string &prefix = "");

protected:
	void create(Poco::Data::Session &session, Identity &identity);
	bool fetch(Poco::Data::Session &session, Identity &identity);
	bool fetchBy(Poco::Data::Session &session, Identity &identity,
			const std::string &email);
	bool remove(Poco::Data::Session &session, const Identity &identity);
};

}
