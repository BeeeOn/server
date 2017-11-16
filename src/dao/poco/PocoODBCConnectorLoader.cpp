#include <Poco/Data/ODBC/Connector.h>

#include "di/Injectable.h"
#include "dao/poco/PocoODBCConnectorLoader.h"

using namespace std;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoODBCConnectorLoader)
BEEEON_OBJECT_CASTABLE(ConnectorLoader)
BEEEON_OBJECT_END(BeeeOn, PocoODBCConnectorLoader)

PocoODBCConnectorLoader::PocoODBCConnectorLoader():
	ConnectorLoader("odbc")
{
	Poco::Data::ODBC::Connector::registerConnector();
}

PocoODBCConnectorLoader::~PocoODBCConnectorLoader()
{
	Poco::Data::ODBC::Connector::unregisterConnector();
}
