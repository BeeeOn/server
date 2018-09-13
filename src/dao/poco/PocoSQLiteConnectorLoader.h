#pragma once

#include "dao/ConnectorLoader.h"

namespace BeeeOn {

class PocoSQLiteConnectorLoader :
	public ConnectorLoader {
public:
	PocoSQLiteConnectorLoader();
	virtual ~PocoSQLiteConnectorLoader();
};

}
