#include <Poco/Data/SQLite/Connector.h>

#include "dao/poco/PocoSQLiteConnectorLoader.h"

using namespace std;
using namespace BeeeOn;

BEEEON_OBJECT(BeeeOn_PocoSQLiteConnectorLoader, BeeeOn::PocoSQLiteConnectorLoader)

PocoSQLiteConnectorLoader::PocoSQLiteConnectorLoader():
	ConnectorLoader("sqlite")
{
	Poco::Data::SQLite::Connector::registerConnector();
}

PocoSQLiteConnectorLoader::~PocoSQLiteConnectorLoader()
{
	Poco::Data::SQLite::Connector::unregisterConnector();
}
