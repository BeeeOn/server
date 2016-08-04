#ifndef BEEEON_GATEWAY_H
#define BEEEON_GATEWAY_H

#include <Poco/SharedPtr.h>
#include "model/Collection.h"

namespace BeeeOn {

/**
 * Representation of a Gateway application entity.
 */
class Gateway {
public:
	typedef Poco::SharedPtr<Gateway> Ptr;
	typedef BeeeOn::Collection<Gateway> Collection;
	typedef long ID;

	Gateway()
	{
	}

	Gateway(const Gateway &copy):
		m_name(copy.m_name)
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
	std::string m_name;
};

typedef Gateway::Collection GatewayCollection;
typedef Gateway::ID GatewayID;

}

#endif
