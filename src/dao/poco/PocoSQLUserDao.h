#include <Poco/SharedPtr.h>

#include "dao/UserDao.h"
#include "dao/Query.h"
#include "dao/poco/PocoAbstractDao.h"
#include "l10n/LocaleManager.h"

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

	void setLocaleManager(LocaleManager::Ptr manager);

	void create(User &user) override;
	bool fetch(User &user) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			User &user, LocaleManager &localeManager,
			const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			User &user, LocaleManager &localeManager,
			const std::string &prefix = "");
	static bool parseIfIDNotNull(Poco::Data::Row &result,
			User &user, LocaleManager &localeManager,
			const std::string &prefix = "");
	static bool parseIfIDNotNull(Poco::Data::Row &result,
			User &user, const std::string &prefix = "");

protected:
	static bool doParseIfIDNotNull(Poco::Data::Row &result,
			User &user, const std::string &prefix = "");

private:
	LocaleManager::Ptr m_localeManager;
	Query m_queryCreate {"users.create"};
	Query m_queryFetch  {"users.fetch_by_id"};
};

}
