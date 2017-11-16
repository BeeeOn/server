#include <Poco/Data/SQLite/Connector.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSQLiteConnectorLoader.h"

using namespace std;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLiteConnectorLoader)
BEEEON_OBJECT_CASTABLE(ConnectorLoader)
BEEEON_OBJECT_END(BeeeOn, PocoSQLiteConnectorLoader)

PocoSQLiteConnectorLoader::PocoSQLiteConnectorLoader():
	ConnectorLoader("sqlite")
{
	Poco::Data::SQLite::Connector::registerConnector();
}

PocoSQLiteConnectorLoader::~PocoSQLiteConnectorLoader()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}
