#ifndef BEEEON_WORK_H
#define BEEEON_WORK_H

#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>
#include <Poco/Timespan.h>
#include <Poco/Nullable.h>

#include "model/Entity.h"
#include "model/GlobalID.h"
#include "work/WorkContent.h"

namespace BeeeOn {

class Work : public Entity<GlobalID> {
public:
	typedef Poco::SharedPtr<Work> Ptr;

	enum State {
		STATE_IDLE,
		STATE_SCHEDULED,
		STATE_EXECUTED,
		STATE_FINISHED,
		STATE_FAILED,
		STATE_CANCELED,
	};

	Work();
	Work(const ID &id);
	virtual ~Work();

	void setState(State state);
	State state() const;

	void setPriority(int priority);

	/**
	 * Work priority. The higher it is the more probable to execute earlier.
	 * The default priority is 0. Thus negative priorities potentially slows down.
	 */
	int priority() const;

	/**
	 * Return when this work is to be activated.
	 * Is the work has been suspended, activate it at
	 * suspended() + sleepDuration(). Otherwise, activate
	 * it as close to created() as possible.
	 */
	Poco::Nullable<Poco::Timestamp> activationTime() const;

	void setNoSleepDuration();
	void setSleepDuration(const Poco::Timespan &duration);
	void setSleepDuration(const Poco::Nullable<Poco::Timespan> &duration);
	Poco::Nullable<Poco::Timespan> sleepDuration() const;

	void setCreated(const Poco::Timestamp &created);
	Poco::Timestamp created() const;

	void setSuspended(const Poco::Timestamp &suspended);
	Poco::Timestamp suspended() const;

	void setFinished(const Poco::Timestamp &finished);
	Poco::Timestamp finished() const;

	void setContent(const WorkContent &content);
	WorkContent &content();
	WorkContent content() const;

	template <typename T>
	T contentAs() const
	{
		return T(m_content);
	}

	static bool timestampValid(const Poco::Timestamp &tstamp)
	{
		return tstamp.epochMicroseconds() != 0;
	}

protected:
	Poco::Nullable<Poco::Timestamp> activationTimeUnlocked() const;

private:
	State m_state;
	int m_priority;
	Poco::Nullable<Poco::Timespan> m_sleepDuration;
	Poco::Timestamp m_created;
	Poco::Timestamp m_suspended;
	Poco::Timestamp m_finished;
	WorkContent m_content;
};

typedef Work::ID WorkID;

}

#endif
