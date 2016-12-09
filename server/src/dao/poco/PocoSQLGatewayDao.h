#include "dao/GatewayDao.h"
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

class PocoSQLGatewayDao :
		public PocoAbstractDao,
		public GatewayDao {
public:
	bool insert(Gateway &gateway) override;
	bool fetch(Gateway &gateway) override;
	bool update(Gateway &gateway) override;
	bool assignAndUpdate(Gateway &gateway, const Place &place) override;
	bool assign(Gateway &gateway, const Place &place) override;
	bool unassign(Gateway &gateway) override;
	bool fetchFromPlace(Gateway &gateway, const Place &place) override;
	void fetchAccessible(
			std::vector<Gateway> &gateways,
			const User &user) override;

	template <typename G>
	static bool parseSingle(Poco::Data::RecordSet &result,
			G &gateway, const std::string &prefix = "")
	{
		if (result.begin() == result.end())
			return false;

		return parseSingle(*result.begin(), gateway, prefix);
	}

	static bool parseSingle(Poco::Data::Row &result,
			Gateway &gateway, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			LegacyGateway &gateway, const std::string &prefix = "");

	template <typename G, typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection)
	{
		for (auto row : result) {
			G gateway;

			if (!parseSingle(row, gateway)) {
				LOGGER_FUNC(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(gateway);
		}
	}


protected:
	bool insert(Poco::Data::Session &session, Gateway &gateway);
	bool fetch(Poco::Data::Session &session, Gateway &gateway);
	bool update(Poco::Data::Session &session, Gateway &gateway);
	bool assignAndUpdate(Poco::Data::Session &session,
			Gateway &gateway, const Place &place);
	bool assign(Poco::Data::Session &session,
			Gateway &gateway, const Place &place);
	bool unassign(Poco::Data::Session &session, Gateway &gateway);
	bool fetchFromPlace(Poco::Data::Session &session,
			Gateway &gateway, const Place &place);
	void fetchAccessible(Poco::Data::Session &session,
			std::vector<Gateway> &gateways,
			const User &user);
};

}
