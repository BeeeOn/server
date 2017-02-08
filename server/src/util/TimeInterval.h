#ifndef BEEEON_TIME_INTERVAL_H
#define BEEEON_TIME_INTERVAL_H

#include <Poco/Timestamp.h>

namespace BeeeOn {

/**
 * Interval between two timestamps. The start must always be
 * less or equal to end. The duration of the interval is
 * defined as:
 *
 *    m_end - m_start
 *
 * The m_end is not part of the interval.
 */
class TimeInterval {
public:
	TimeInterval(const Poco::Timestamp &start,
			const Poco::Timestamp &end);
	~TimeInterval();

	/**
	 * The time interval is empty (start == end).
	 */
	bool isEmpty() const;

	/**
	 * The time interval both starts and ends before
	 * the given timestampt at.
	 */
	bool isBefore(const Poco::Timestamp &at) const;

	Poco::Timestamp start() const;
	Poco::Timestamp end() const;

	const Poco::Timestamp &start();
	const Poco::Timestamp &end();

private:
	Poco::Timestamp m_start;
	Poco::Timestamp m_end;
};

}

#endif
