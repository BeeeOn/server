#include "dao/VerifiedIdentityDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"
#include "util/Loggable.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLVerifiedIdentityDao :
		public PocoAbstractDao,
		public VerifiedIdentityDao {
public:
	PocoSQLVerifiedIdentityDao();

	void create(VerifiedIdentity &identity) override;
	bool fetch(VerifiedIdentity &identity) override;
	bool fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider) override;
	void fetchBy(std::vector<VerifiedIdentity> &identities,
			const std::string email) override;
	void fetchBy(std::list<VerifiedIdentity> &identities,
			const User &user) override;
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
				Loggable::forMethod(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(identity);
		}
	}

private:
	SQLQuery m_queryCreate       {"verified_identities.create"};
	SQLQuery m_queryUpdate       {"verified_identities.update"};
	SQLQuery m_queryRemove       {"verified_identities.remove"};
	SQLQuery m_queryFetchById    {"verified_identities.fetch.by.id"};
	SQLQuery m_queryFetchByEmail {"verified_identities.fetch.by.email"};
	SQLQuery m_queryFetchByEmailAndProvider {"verified_identities.fetch.by.email.and.provider"};
	SQLQuery m_queryFetchByUser  {"verified_identities.fetch.by.user"};
};

}
