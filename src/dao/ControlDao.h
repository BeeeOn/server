#ifndef BEEEON_CONTROL_DAO_H
#define BEEEON_CONTROL_DAO_H

#include <list>

#include <Poco/SharedPtr.h>

#include "model/Control.h"

namespace BeeeOn {

class Device;

class ControlDao {
public:
	typedef Poco::SharedPtr<ControlDao> Ptr;

	virtual ~ControlDao();

	virtual bool fetch(Control &control,
			const Device &device) = 0;
	virtual void fetchBy(std::list<Control> &control,
			const Device &device) = 0;
	virtual bool insert(const Control::RequestedValue &request,
			const Control &control,
			const Device &device) = 0;
	virtual bool update(const Control::RequestedValue &request,
			const Control &control,
			const Device &device) = 0;
};

}

#endif
