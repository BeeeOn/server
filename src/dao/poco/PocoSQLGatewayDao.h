#include "dao/GatewayDao.h"
#include "dao/Query.h"
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
	Query m_queryCreate          {"gateways.create"};
	Query m_queryUpdate          {"gateways.update"};
	Query m_queryFetchById       {"gateways.fetch_by_id"};
	Query m_queryFetchAccessible {"gateways.fetch_accessible"};
	Query m_queryLegacyFetchById {"legacy_gateways.fetch_by_id"};
	Query m_queryLegacyFetchAccessible {"legacy_gateways.fetch_accessible"};

	TimeZoneProvider::Ptr m_timeZoneProvider;
};

}
