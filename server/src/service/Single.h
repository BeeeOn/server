#ifndef BEEEON_SINGLE_H
#define BEEEON_SINGLE_H

#include "service/Deserializer.h"

namespace BeeeOn {

/**
 * Single entity representation.
 */
template <typename T>
class Single {
public:
	Single(T &target):
		m_target(target)
	{
	}

	T &target()
	{
		return m_target;
	}

private:
	T &m_target;
};

/**
 * Single entity representation with input data.
 */
template <typename T>
class SingleWithData : public Single<T> {
public:
	SingleWithData(T &target, const Deserializer<T> &data):
		Single<T>(target),
		m_data(data)
	{
	}

	const Deserializer<T> &data() const
	{
		return m_data;
	}

private:
	const Deserializer<T> &m_data;
};

}

#endif

