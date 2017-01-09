#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>

#include "dao/poco/PocoSQLPlaceDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLPlaceDao, BeeeOn::PocoSQLPlaceDao)

PocoSQLPlaceDao::PocoSQLPlaceDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryRemove);
	registerQuery(m_queryFetchById);
}

void PocoSQLPlaceDao::create(Place &place)
{
	place = Place(PlaceID::random(), place);
	string id(place.id().toString());
	string name(place.name());

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(name, "name")
	);

	execute(sql);
}

bool PocoSQLPlaceDao::fetch(Place &place)
{
	assureHasId(place);
	string id(place.id().toString());

	Statement sql = (session() << m_queryFetchById(),
		use(id, "id")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, place);
}

bool PocoSQLPlaceDao::update(Place &place)
{
	assureHasId(place);
	string id(place.id().toString());
	string name(place.name());

	Statement sql = (session() << m_queryUpdate(),
		use(name, "name"),
		use(id, "id")
	);

	return execute(sql) > 0;
}

bool PocoSQLPlaceDao::remove(const Place &place)
{
	assureHasId(place);
	string id(place.id().toString());

	Statement sql = (session() << m_queryRemove(),
		use(id, "id")
	);

	return execute(sql) > 0;
}

bool PocoSQLPlaceDao::parseSingle(RecordSet &result,
		Place &place, const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), place, prefix);
}

bool PocoSQLPlaceDao::parseSingle(Row &result,
		Place &place, const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		place = Place(PlaceID::parse(result[prefix + "id"]));

	place.setName(emptyWhenNull(result[prefix + "name"]));
	return true;
}

bool PocoSQLPlaceDao::parseIfIDNotNull(Row &result,
			Place &place, const string &prefix)
{
	const string id = emptyWhenNull(result[prefix + "id"]);
	if (id.empty())
		return false;

	place = Place(PlaceID::parse(id));
	place.setName(result[prefix + "name"]);
	return true;
}
