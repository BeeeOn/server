#ifndef BEEEON_CONTROL_SERVICE_IMPL_H
#define BEEEON_CONTROL_SERVICE_IMPL_H

#include <Poco/SharedPtr.h>

#include "dao/ControlDao.h"
#include "dao/DeviceDao.h"
#include "policy/ControlAccessPolicy.h"
#include "service/ControlService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class ControlServiceImpl : public ControlService, public Transactional {
public:
	typedef Poco::SharedPtr<ControlServiceImpl> Ptr;

	void setDeviceDao(DeviceDao::Ptr dao);
	void setControlDao(ControlDao::Ptr dao);
	void setAccessPolicy(ControlAccessPolicy::Ptr policy);

	bool fetch(Relation<Control, Device> &data) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(data));
	}

	void fetchMany(Relation<std::list<Control>, Device> &data) override
	{
		BEEEON_TRANSACTION(doFetchMany(data));
	}

protected:
	bool doFetch(Relation<Control, Device> &data);
	void doFetchMany(Relation<std::list<Control>, Device> &data);

private:
	DeviceDao::Ptr m_deviceDao;
	ControlDao::Ptr m_controlDao;
	ControlAccessPolicy::Ptr m_accessPolicy;
};

}

#endif

