#include "dao/PlaceDao.h"
#include "dao/SQLQuery.h"
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

class PocoSQLPlaceDao :
		public PocoAbstractDao,
		public PlaceDao {
public:
	PocoSQLPlaceDao();

	void create(Place &place) override;
	bool fetch(Place &place) override;
	bool update(Place &place) override;
	bool remove(const Place &place) override;

	static bool parseSingle(Poco::Data::RecordSet &result,
			Place &place, const std::string &prefix = "");
	static bool parseSingle(Poco::Data::Row &result,
			Place &place, const std::string &prefix = "");
	static bool parseIfIDNotNull(Poco::Data::Row &result,
			Place &place, const std::string &prefix = "");

	template <typename C>
	static void parseMany(Poco::Data::RecordSet &result, C &collection)
	{
		for (auto row : result) {
			Place place;

			if (!parseSingle(row, place)) {
				LOGGER_FUNC(__func__)
					.warning("skipping malformed data, query result: "
						+ row.valuesToString(), __FILE__, __LINE__);
				continue;
			}

			collection.push_back(place);
		}
	}

protected:
	void create(Poco::Data::Session &session, Place &place);
	bool fetch(Poco::Data::Session &session, Place &place);
	bool update(Poco::Data::Session &session, Place &place);
	bool remove(Poco::Data::Session &session, const Place &place);

private:
	SQLQuery m_queryCreate {"places.create"};
	SQLQuery m_queryUpdate {"places.update"};
	SQLQuery m_queryRemove {"places.remove"};
	SQLQuery m_queryFetchById {"places.fetch.by.id"};
};

}
