#include "dao/IdentityDao.h"
#include "dao/SQLQuery.h"
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
	PocoSQLIdentityDao();

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

private:
	SQLQuery m_queryCreate       {"identities.create"};
	SQLQuery m_queryRemove       {"identities.remove"};
	SQLQuery m_queryFetchById    {"identities.fetch.by.id"};
	SQLQuery m_queryFetchByEmail {"identities.fetch.by.email"};
};

}
