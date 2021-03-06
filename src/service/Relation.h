#pragma once

#include "service/Deserializer.h"
#include "policy/PolicyContext.h"

namespace BeeeOn {

/**
 * Relation from a target to its base.
 */
template <typename T, typename Base>
class Relation : public UserPolicyContext {
public:
	Relation(T &target, const Base &base):
		m_target(target),
		m_base(base)
	{
	}

	T &target()
	{
		return m_target;
	}

	const T &target() const
	{
		return m_target;
	}

	const Base &base() const
	{
		return m_base;
	}

private:
	T &m_target;
	const Base &m_base;
};

/**
 * Relation from a target to its base. While creating or changing the
 * relation, a data update of the target is also considered.
 */
template <typename T, typename Base>
class RelationWithData : public Relation<T, Base> {
public:
	RelationWithData(T &target, const Deserializer<T> &data,
			const Base &base):
		Relation<T, Base>(target, base),
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
