#include "dao/VerifiedIdentityDao.h"
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

class PocoSQLVerifiedIdentityDao :
		public PocoAbstractDao,
		public VerifiedIdentityDao {
public:
	void create(VerifiedIdentity &identity) override;
	bool fetch(VerifiedIdentity &identity) override;
	bool fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider) override;
	void fetchBy(std::vector<VerifiedIdentity> &identities,
			const std::string email) override;
	bool update(VerifiedIdentity &identity) override;
	bool remove(const VerifiedIdentity &identity) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			VerifiedIdentity &identity, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			VerifiedIdentity &identity, const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection)
	{
		for (auto row : result) {
			VerifiedIdentity identity;

			if (!parseSingle(row, identity)) {
				LOGGER_FUNC(__func__
					).warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(identity);
		}
	}

protected:
	void create(Poco::Data::Session &session, VerifiedIdentity &identity);
	bool fetch(Poco::Data::Session &session, VerifiedIdentity &identity);
	bool fetchBy(Poco::Data::Session &session, VerifiedIdentity &identity,
			const std::string &email,
			const std::string &provider);
	void fetchBy(Poco::Data::Session &session, 
			std::vector<VerifiedIdentity> &identities,
			const std::string &email);
	bool update(Poco::Data::Session &session, VerifiedIdentity &identity);
	bool remove(Poco::Data::Session &session, const VerifiedIdentity &identity);
};

}
