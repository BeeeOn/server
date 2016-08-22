#ifndef BEEEON_MOCK_INIT_H
#define BEEEON_MOCK_INIT_H

#include "di/InjectorTarget.h"
#include "dao/UserDao.h"
#include "dao/GatewayDao.h"

namespace BeeeOn {

class UIMockInit : public AbstractInjectorTarget {
public:
	UIMockInit()
	{
		injector<UIMockInit, MockUserDao>("mockUserDao",
			&UIMockInit::setMockUserDao);
		injector<UIMockInit, MockGatewayDao>("mockGatewayDao",
			&UIMockInit::setMockGatewayDao);
	}

	void setMockUserDao(MockUserDao *dao)
	{
		m_userDao = dao;
	}

	void setMockGatewayDao(MockGatewayDao *dao)
	{
		m_gatewayDao = dao;
	}

protected:
	void injectionDone();

private:
	MockUserDao *m_userDao;
	MockGatewayDao *m_gatewayDao;
};

}

#endif
