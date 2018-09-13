#pragma once

#include "dao/ConnectorLoader.h"

namespace BeeeOn {

class PocoODBCConnectorLoader : public ConnectorLoader {
public:
	PocoODBCConnectorLoader();
	virtual ~PocoODBCConnectorLoader();
};

}
