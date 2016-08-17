#ifndef BEEEON_MOCK_INIT_H
#define BEEEON_MOCK_INIT_H

#include "di/InjectorTarget.h"
#include "dao/UserDao.h"

namespace BeeeOn {

class UIMockInit : public AbstractInjectorTarget {
public:
	UIMockInit()
	{
		injector<UIMockInit, MockUserDao>("mockUserDao",
			&UIMockInit::setMockUserDao);
	}

	void setMockUserDao(MockUserDao *dao)
	{
		m_userDao = dao;
	}

protected:
	void injectionDone();

private:
	MockUserDao *m_userDao;
};

}

#endif
