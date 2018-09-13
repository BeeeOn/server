#pragma once

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class Device;
class Control;
class PolicyContext;

class ControlAccessPolicy {
public:
	typedef Poco::SharedPtr<ControlAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_GET,
		ACTION_USER_SET,
	};

	virtual ~ControlAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Control &control,
		const Device &device) = 0;

	virtual void assureMany(
		const Action action,
		const PolicyContext &context,
		const Device &device) = 0;
};

}
