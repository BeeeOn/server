#ifndef BEEEON_CONTROL_SERVICE_H
#define BEEEON_CONTROL_SERVICE_H

#include <Poco/SharedPtr.h>

#include "model/Control.h"
#include "model/Device.h"
#include "service/Relation.h"
#include "work/Work.h"

namespace BeeeOn {

class ControlService {
public:
	typedef Poco::SharedPtr<ControlService> Ptr;

	virtual ~ControlService();

	virtual bool fetch(Relation<Control, Device> &data) = 0;
	virtual void fetchMany(Relation<std::list<Control>, Device> &data) = 0;
};

}

#endif
