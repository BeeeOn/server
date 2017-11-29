#pragma once

#include "dao/FCMTokenDao.h"
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

class PocoSQLFCMTokenDao :
		public PocoAbstractDao,
		public FCMTokenDao {
public:
	PocoSQLFCMTokenDao();

	void insert(FCMToken &token) override;
	void fetchBy(std::vector<FCMToken> &token, const User &user) override;
	bool fetch(FCMToken &token) override;
	bool replace(const FCMToken &tokenFrom, FCMToken &tokenTo) override;
	bool remove(FCMToken &token) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			FCMToken &token, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			FCMToken &token, const std::string &prefix = "");

	static bool parseSingle(Poco::Data::RecordSet &result,
			FCMToken &token, const User &user, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			FCMToken &token, const User &user, const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, const User &user, C &collection)
	{
		for (auto row : result) {
			FCMToken token;

			if (!parseSingle(row, token, user)) {
				Loggable::forMethod(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(token);
		}
	}

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection)
	{
		for (auto row : result) {
			FCMToken token;

			if (!parseSingle(row, token)) {
				Loggable::forMethod(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(token);
		}
	}

private:
	SQLQuery m_queryCreate              {"fcm_tokens.create"};
	SQLQuery m_queryReplace             {"fcm_tokens.replace"};
	SQLQuery m_queryRemove              {"fcm_tokens.remove"};
	SQLQuery m_queryFetchByUserId       {"fcm_tokens.fetch.by.user.id"};
	SQLQuery m_queryFetchUserByFCMToken {"fcm_tokens.fetch.by.id"};
};

}
