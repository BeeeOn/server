#pragma once

#include "dao/ConnectorLoader.h"

namespace BeeeOn {

/**
 * @brief Loads cassandra connector which enable connection to Cassandra database
 */
class PocoCassandraConnectorLoader :
	public ConnectorLoader {
public:
	PocoCassandraConnectorLoader();
	~PocoCassandraConnectorLoader();
};

}

