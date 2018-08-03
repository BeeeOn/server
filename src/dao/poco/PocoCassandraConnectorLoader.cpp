#include <Poco/Data/Cassandra/Connector.h>

#include "di/Injectable.h"
#include "dao/poco/PocoCassandraConnectorLoader.h"

using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoCassandraConnectorLoader)
BEEEON_OBJECT_CASTABLE(ConnectorLoader)
BEEEON_OBJECT_END(BeeeOn, PocoCassandraConnectorLoader)

PocoCassandraConnectorLoader::PocoCassandraConnectorLoader():
	ConnectorLoader("cassandra")
{
	Poco::Data::Cassandra::Connector::registerConnector();
}

PocoCassandraConnectorLoader::~PocoCassandraConnectorLoader()
{
	Poco::Data::Cassandra::Connector::unregisterConnector();
}
