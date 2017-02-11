#ifndef BEEEON_POCO_SQLITE_CONNECTOR_LOADER_H
#define BEEEON_POCO_SQLITE_CONNECTOR_LOADER_H

#include "di/AbstractInjectorTarget.h"
#include "dao/ConnectorLoader.h"

namespace BeeeOn {

class PocoSQLiteConnectorLoader :
	public ConnectorLoader,
	public AbstractInjectorTarget {
public:
	PocoSQLiteConnectorLoader();
	virtual ~PocoSQLiteConnectorLoader();
};

}

#endif
