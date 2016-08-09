#ifndef BEEEON_GLOBAL_ID_H
#define BEEEON_GLOBAL_ID_H

#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>

namespace BeeeOn {

class GlobalID {
public:
	GlobalID()
	{
	}

	GlobalID(const Poco::UUID &uuid):
		m_uuid(uuid)
	{
	}

	GlobalID(const GlobalID &gid):
		m_uuid(gid.m_uuid)
	{
	}

	bool isNull() const
	{
		return m_uuid.isNull();
	}

	static GlobalID parse(const std::string &s)
	{
		GlobalID gid;
		gid.m_uuid.parse(s);
		return gid;
	}

	std::string toString() const
	{
		return m_uuid.toString();
	}

	bool operator !=(const GlobalID &id) const
	{
		return m_uuid != id.m_uuid;
	}

	bool operator ==(const GlobalID &id) const
	{
		return m_uuid == id.m_uuid;
	}

	bool operator <(const GlobalID &id) const
	{
		return m_uuid < id.m_uuid;
	}

	bool operator >(const GlobalID &id) const
	{
		return m_uuid > id.m_uuid;
	}

	bool operator <=(const GlobalID &id) const
	{
		return m_uuid <= id.m_uuid;
	}

	bool operator >=(const GlobalID &id) const
	{
		return m_uuid >= id.m_uuid;
	}

private:
	Poco::UUID m_uuid;
};

};

#endif
