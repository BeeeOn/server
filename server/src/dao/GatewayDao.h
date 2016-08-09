#ifndef BEEEON_GATEWAY_DAO_H
#define BEEEON_GATEWAY_DAO_H

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/Place.h"
#include "model/Gateway.h"

namespace BeeeOn {

class GatewayDao {
public:
	virtual bool fetch(Gateway &gateway) = 0;
};

class NullGatewayDao : public AbstractInjectorTarget,
	public NullDao<Gateway, GatewayDao> {
};

class MockGatewayDao : public AbstractInjectorTarget,
	public MockDao<Gateway, GatewayDao> {
protected:
	GatewayID nextID()
	{
		return ++m_id;
	}

private:
	GatewayID m_id;
};

}

#endif
