#include <Poco/Data/SQLite/Connector.h>

#include "dao/PocoSQLiteConnectorLoader.h"

using namespace std;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLiteConnectorLoader, BeeeOn::PocoSQLiteConnectorLoader)

PocoSQLiteConnectorLoader::PocoSQLiteConnectorLoader():
	ConnectorLoader("sqlite")
{
	Poco::Data::SQLite::Connector::registerConnector();
}

PocoSQLiteConnectorLoader::~PocoSQLiteConnectorLoader()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}
