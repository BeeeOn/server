#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RowIterator.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RecordSet.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSQLFCMTokenDao.h"
#include "dao/poco/PocoSQLIdentityDao.h"
#include "dao/poco/PocoSQLUserDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLFCMTokenDao)
BEEEON_OBJECT_CASTABLE(FCMTokenDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLFCMTokenDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLFCMTokenDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLFCMTokenDao::setQueryLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLFCMTokenDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLFCMTokenDao)

PocoSQLFCMTokenDao::PocoSQLFCMTokenDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryReplace);
	registerQuery(m_queryFetchByUserId);
	registerQuery(m_queryFetchUserByFCMToken);
	registerQuery(m_queryRemove);
}

void PocoSQLFCMTokenDao::insert(FCMToken &token)
{
	assureHasId(token.user());

	string userID(token.user().id().toString());
	string tokenValue(token.id().toString());

	Statement sql = (session() << m_queryCreate(),
		use(tokenValue, "token"),
		use(userID, "user_id")
	);

	execute(sql);
}

void PocoSQLFCMTokenDao::fetchBy(
		std::vector<FCMToken> &tokens,
		const User &user)
{
	assureHasId(user);

	string searchUserId(user.id().toString());

	Statement sql = (session() << m_queryFetchByUserId(),
		use(searchUserId, "user_id")
	);

	RecordSet result = executeSelect(sql);
	parseMany(result, user, tokens);
}

bool PocoSQLFCMTokenDao::fetch(FCMToken &token)
{
	string tokenValue(token.id().toString());

	Statement sql = (session() << m_queryFetchUserByFCMToken(),
		use(tokenValue, "token")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	return parseSingle(result, token);
}

bool PocoSQLFCMTokenDao::replace(const FCMToken &tokenFrom, FCMToken &tokenTo)
{
	string tokenFromValue(tokenFrom.id().toString());
	string tokenToValue(tokenTo.id().toString());

	Statement sql = (session() << m_queryReplace(),
		use(tokenFromValue, "token_from"),
		use(tokenToValue, "token_to")
	);

	return execute(sql) > 0;
}

bool PocoSQLFCMTokenDao::remove(FCMToken &token)
{
	string tokenValue(token.id().toString());

	Statement sql = (session() << m_queryRemove(),
		use(tokenValue, "token")
	);

	return execute(sql) > 0;
}

bool PocoSQLFCMTokenDao::parseSingle(RecordSet &result,
		FCMToken &token, const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), token, prefix);
}

bool PocoSQLFCMTokenDao::parseSingle(Row &result,
		FCMToken &token, const string &prefix)
{
	User user(UserID::parse(result[prefix + "user_id"]));
	return parseSingle(result, token, user, prefix);
}

bool PocoSQLFCMTokenDao::parseSingle(RecordSet &result,
		FCMToken &token, const User &user, const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), token, user, prefix);
}

bool PocoSQLFCMTokenDao::parseSingle(Row &result,
		FCMToken &token, const User &user, const string &prefix)
{
	if (!hasColumn(result, prefix + "token"))
		return false;

	token.setId(FCMTokenID::parse(result[prefix + "token"].convert<string>()));
	token.setUser(user);

	return true;
}
