#include "dao/PlaceDao.h"
#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class Session;

}
}

namespace BeeeOn {

class PocoSQLPlaceDao :
		public PocoAbstractDao,
		public PlaceDao {
public:
	void create(Place &place) override;
	bool fetch(Place &place) override;
	bool update(Place &place) override;
	bool remove(const Place &place) override;

protected:
	void create(Poco::Data::Session &session, Place &place);
	bool fetch(Poco::Data::Session &session, Place &place);
	bool update(Poco::Data::Session &session, Place &place);
	bool remove(Poco::Data::Session &session, const Place &place);
};

}
