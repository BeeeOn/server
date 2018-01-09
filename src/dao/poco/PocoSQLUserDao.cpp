#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSQLUserDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "l10n/LocaleManager.h"
#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLUserDao)
BEEEON_OBJECT_CASTABLE(UserDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLUserDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLUserDao::setTransactionManager)
BEEEON_OBJECT_REF("localeManager", &PocoSQLUserDao::setLocaleManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLUserDao::setIniFileQueryLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLUserDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLUserDao)

PocoSQLUserDao::PocoSQLUserDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryFetch);
}

void PocoSQLUserDao::setLocaleManager(SharedPtr<LocaleManager> manager)
{
	m_localeManager = manager;
}

void PocoSQLUserDao::create(User &user)
{
	user.setId(UserID::random());
	string id(user.id().toString());
	string firstName(user.firstName());
	string lastName(user.lastName());
	string locale(user.locale().toString());

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(firstName),
		use(lastName),
		use(locale)
	);

	execute(sql);
}

bool PocoSQLUserDao::fetch(User &user)
{
	assureHasId(user);
	string id(user.id().toString());

	Statement sql = (session() << m_queryFetch(),
		use(id, "id")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	return parseSingle(result, user, *m_localeManager);
}

bool PocoSQLUserDao::parseSingle(
		RecordSet &result,
		User &user,
		LocaleManager &localeManager,
		const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), user, localeManager, prefix);
}

bool PocoSQLUserDao::parseSingle(
		Row &result,
		User &user,
		LocaleManager &localeManager,
		const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		user.setId(UserID::parse(result[prefix + "id"]));

	user.setFirstName(result[prefix + "first_name"]);
	user.setLastName(result[prefix + "last_name"]);
	user.setLocale(localeManager.parse(result[prefix + "locale"]));

	markLoaded(user);
	return true;
}

bool PocoSQLUserDao::doParseIfIDNotNull(
		Row &result,
		User &user,
		const string &prefix)
{
	const string id = emptyWhenNull(result[prefix + "id"]);
	if (id.empty())
		return false;

	user.setId(UserID::parse(id));
	user.setFirstName(result[prefix + "first_name"]);
	user.setLastName(result[prefix + "last_name"]);

	return true;
}

bool PocoSQLUserDao::parseIfIDNotNull(Row &result,
			User &user,
			LocaleManager &localeManager,
			const string &prefix)
{
	if (!doParseIfIDNotNull(result, user, prefix))
		return false;

	user.setLocale(localeManager.parse(result[prefix + "locale"]));
	markLoaded(user);
	return true;
}

bool PocoSQLUserDao::parseIfIDNotNull(
		Row &result,
		User &user,
		const string &prefix)
{
	if (!doParseIfIDNotNull(result, user, prefix))
		return false;

	markLoaded(user);
	return true;
}
