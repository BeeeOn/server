#ifndef BEEEON_CONNECTOR_LOADER_H
#define BEEEON_CONNECTOR_LOADER_H

#include <string>

namespace BeeeOn {

class ConnectorLoader {
public:
	ConnectorLoader(const std::string &name);
	virtual ~ConnectorLoader();

	const std::string &name() const
	{
		return m_name;
	}

	static ConnectorLoader &null();

private:
	std::string m_name;
};

}

#endif
