#include "dao/GatewayDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"
#include "l10n/TimeZoneProvider.h"
#include "util/Loggable.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLGatewayDao :
		public PocoAbstractDao,
		public GatewayDao {
public:
	PocoSQLGatewayDao();

	void setTimeZoneProvider(TimeZoneProvider::Ptr provider);

	bool insert(Gateway &gateway) override;
	bool fetch(Gateway &gateway) override;
	bool fetch(LegacyGateway &gateway, const User &user) override;
	bool update(Gateway &gateway) override;
	void fetchAccessible(
			std::vector<Gateway> &gateways,
			const User &user) override;
	void fetchAccessible(
			std::vector<LegacyGateway> &gateways,
			const User &user) override;

	template <typename G>
	static bool parseSingle(Poco::Data::RecordSet &result,
			TimeZoneProvider::Ptr provider,
			G &gateway, const std::string &prefix = "")
	{
		if (result.begin() == result.end())
			return false;

		return parseSingle(*result.begin(), provider, gateway, prefix);
	}

	static bool parseSingle(Poco::Data::Row &result,
			TimeZoneProvider::Ptr provider,
			Gateway &gateway, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			TimeZoneProvider::Ptr provider,
			LegacyGateway &gateway, const std::string &prefix = "");

	template <typename G, typename C>
	static void parseMany(Poco::Data::RecordSet &result, TimeZoneProvider::Ptr provider, C &collection)
	{
		for (auto row : result) {
			G gateway;

			if (!parseSingle(row, provider, gateway)) {
				Loggable::forMethod(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(gateway);
		}
	}

private:
	SQLQuery m_queryCreate          {"gateways.create"};
	SQLQuery m_queryUpdate          {"gateways.update"};
	SQLQuery m_queryFetchById       {"gateways.fetch.by.id"};
	SQLQuery m_queryFetchAccessible {"gateways.fetch.accessible"};
	SQLQuery m_queryLegacyFetchById {"legacy_gateways.fetch.by.id"};
	SQLQuery m_queryLegacyFetchAccessible {"legacy_gateways.fetch.accessible"};

	TimeZoneProvider::Ptr m_timeZoneProvider;
};

}
