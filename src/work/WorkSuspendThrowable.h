#ifndef BEEEON_WORK_SUSPEND_THROWABLE_H
#define BEEEON_WORK_SUSPEND_THROWABLE_H

#include <Poco/Timespan.h>

namespace BeeeOn {

class WorkSuspendThrowable {
public:
	WorkSuspendThrowable(const Poco::Timespan &duration = Poco::Timespan(0, 0));

	Poco::Timespan duration() const;

private:
	Poco::Timespan m_duration;
};

class WorkSuspendForEventThrowable {
public:
	WorkSuspendForEventThrowable();
};

}

#endif
