#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "model/Control.h"
#include "model/Device.h"
#include "service/Relation.h"

namespace BeeeOn {

class ControlService {
public:
	typedef Poco::SharedPtr<ControlService> Ptr;

	virtual ~ControlService();

	virtual bool fetch(Relation<Control, Device> &data) = 0;
	virtual void fetchMany(Relation<std::list<Control>, Device> &data) = 0;
	virtual void requestChange(Relation<Control, Device> &data,
			double value,
			const Poco::Timespan &timeout) = 0;
};

}
