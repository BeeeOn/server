#include "dao/GatewayDao.h"
#include "dao/poco/PocoAbstractDao.h"

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

	static bool parseSingle(Poco::Data::RecordSet &result,
			Gateway &gateway, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			Gateway &gateway, const std::string &prefix = "");

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
