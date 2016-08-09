#ifndef BEEEON_GATEWAY_H
#define BEEEON_GATEWAY_H

#include <Poco/SharedPtr.h>
#include "model/SimpleID.h"
#include "model/Collection.h"

namespace BeeeOn {

/**
 * Representation of a Gateway application entity.
 */
class Gateway {
public:
	typedef Poco::SharedPtr<Gateway> Ptr;
	typedef BeeeOn::Collection<Gateway> Collection;
	typedef SimpleID ID;

	Gateway()
	{
	}

	Gateway(const ID &id):
		m_id(id)
	{
	}

	Gateway(const Gateway &copy):
		m_name(copy.m_name)
	{
	}

	Gateway(const ID &id, const Gateway &copy):
		m_id(id)
	{
	}

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string &name() const
	{
		return m_name;
	}

	const ID &id() const
	{
		return m_id;
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void toWeb(Serializer &s) const
	{
		s.push("name", m_name);
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void fromWeb(Serializer &s)
	{
		s.get("name", m_name);
	}

private:
	ID m_id;
	std::string m_name;
};

typedef Gateway::Collection GatewayCollection;
typedef Gateway::ID GatewayID;

}

#endif