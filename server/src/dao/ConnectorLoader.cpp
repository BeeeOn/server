#include <string>
#include <Poco/SingletonHolder.h>

#include "dao/ConnectorLoader.h"

using namespace std;
using namespace BeeeOn;

ConnectorLoader::ConnectorLoader(const string &name):
	m_name(name)
{
}

ConnectorLoader::~ConnectorLoader()
{
}

namespace BeeeOn {

class NullConnectorLoader : public ConnectorLoader {
public:
	NullConnectorLoader():
		ConnectorLoader("<null>")
	{
	}
};

}

ConnectorLoader &ConnectorLoader::null()
{
	static Poco::SingletonHolder<NullConnectorLoader> singleton;
	return *singleton.get();
}
