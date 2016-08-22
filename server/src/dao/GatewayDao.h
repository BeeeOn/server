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
public:
	/**
	 * Provide a singleton instance to avoid
	 * unnecessary allocations.
	 */
	static GatewayDao &instance();
};

class MockGatewayDao : public AbstractInjectorTarget,
	public MockDao<Gateway, GatewayDao> {
protected:
	GatewayID nextID()
	{
		return GatewayID::random();
	}

private:
	GatewayID m_id;
};

}

#endif
