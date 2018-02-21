#include "dao/VerifiedIdentityDao.h"
#include "dao/Query.h"
#include "dao/poco/PocoAbstractDao.h"
#include "l10n/LocaleManager.h"
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

	void setLocaleManager(LocaleManager::Ptr manager);

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
			VerifiedIdentity &identity,
			LocaleManager::Ptr localeManager,
			const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			VerifiedIdentity &identity,
			LocaleManager::Ptr localeManager,
			const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result,
			LocaleManager::Ptr localeManager,
			C &collection)
	{
		for (auto row : result) {
			VerifiedIdentity identity;

			if (!parseSingle(row, identity, localeManager)) {
				Loggable::forMethod(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(identity);
		}
	}

private:
	Query m_queryCreate       {"verified_identities.create"};
	Query m_queryUpdate       {"verified_identities.update"};
	Query m_queryRemove       {"verified_identities.remove"};
	Query m_queryFetchById    {"verified_identities.fetch_by_id"};
	Query m_queryFetchByEmail {"verified_identities.fetch_by_email"};
	Query m_queryFetchByEmailAndProvider {"verified_identities.fetch_by_email_and_provider"};
	Query m_queryFetchByUser  {"verified_identities.fetch_by_user"};

	LocaleManager::Ptr m_localeManager;
};

}
