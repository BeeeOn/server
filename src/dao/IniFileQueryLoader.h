#ifndef BEEEON_INI_FILE_QUERY_LOADER_H
#define BEEEON_INI_FILE_QUERY_LOADER_H

#include <Poco/AutoPtr.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "dao/QueryLoader.h"

namespace BeeeOn {

class IniFileQueryLoader : public QueryLoader {
public:
	IniFileQueryLoader();
	virtual ~IniFileQueryLoader();

	void addSourceFile(const std::string &path);

	void setDatabase(const std::string &name);

	std::string find(const std::string &key) const override;

	void prepare();

protected:
	Poco::AutoPtr<Poco::Util::LayeredConfiguration> m_config;
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_view;
	std::string m_database;
};

}

#endif
