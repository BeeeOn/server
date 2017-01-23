#include "dao/LocationDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"
#include "Debug.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLLocationDao :
		public PocoAbstractDao,
		public LocationDao {
public:
	PocoSQLLocationDao();

	void create(Location &location) override;
	bool fetch(Location &location) override;
	bool fetchFrom(Location &location,
			const Gateway &gateway) override;
	void fetchBy(std::vector<Location> &locations,
			const Gateway &gateway) override;
	bool update(Location &location) override;
	bool remove(const Location &location) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			Location &location, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			Location &location, const std::string &prefix = "");
	static bool parseIfIDNotNull(Poco::Data::Row &result,
			Location &location, const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection)
	{
		for (auto row : result) {
			Location location;

			if (!parseSingle(row, location)) {
				LOGGER_FUNC(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(location);
		}
	}

private:
	SQLQuery m_queryCreate    {"locations.create"};
	SQLQuery m_queryUpdate    {"locations.update"};
	SQLQuery m_queryRemove    {"locations.remove"};
	SQLQuery m_queryFetchById {"locations.fetch.by.id"};
	SQLQuery m_queryFetchByIdAndGatewayId {"locations.fetch.by.id.and.gateway_id"};
	SQLQuery m_queryFetchByGatewayId {"locations.fetch.by.gateway_id"};
};

}
