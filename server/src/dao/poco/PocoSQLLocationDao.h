#include "dao/LocationDao.h"
#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class Session;

}
}

namespace BeeeOn {

class PocoSQLLocationDao :
		public PocoAbstractDao,
		public LocationDao {
public:
	void create(Location &location) override;
	bool fetch(Location &location) override;
	bool fetchFrom(Location &location,
			const Place &place) override;
	bool fetchFrom(Location &location,
			const Gateway &gateway) override;
	void fetchBy(std::vector<Location> &locations,
			const Place &place) override;
	void fetchBy(std::vector<Location> &locations,
			const Gateway &gateway) override;
	bool update(Location &location) override;
	bool remove(const Location &location) override;

protected:
	void create(Poco::Data::Session &session, Location &location);
	bool fetch(Poco::Data::Session &session, Location &location);
	bool fetchFrom(Poco::Data::Session &session, Location &location,
			const Place &place);
	bool fetchFrom(Poco::Data::Session &session, Location &location,
			const Gateway &gateway);
	void fetchBy(Poco::Data::Session &session, std::vector<Location> &locations,
			const Place &place);
	void fetchBy(Poco::Data::Session &session, std::vector<Location> &locations,
			const Gateway &gateway);
	bool update(Poco::Data::Session &session, Location &location);
	bool remove(Poco::Data::Session &session, const Location &location);
};

}
