#ifndef BEEEON_POCO_ODBC_CONNECTOR_LOADER_H
#define BEEEON_POCO_ODBC_CONNECTOR_LOADER_H

#include "dao/ConnectorLoader.h"

namespace BeeeOn {

class PocoODBCConnectorLoader : public ConnectorLoader {
public:
	PocoODBCConnectorLoader();
	virtual ~PocoODBCConnectorLoader();
};

}

#endif
