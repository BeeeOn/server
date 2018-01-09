#pragma once

#include <string>

#include <Poco/AutoPtr.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/LayeredConfiguration.h>

#include "dao/QueryLoader.h"

namespace BeeeOn {

class AbstractConfigurationQueryLoader : public QueryLoader {
public:
	AbstractConfigurationQueryLoader();

	void addSourceFile(const std::string &path);
	void setDatabase(const std::string &name);
	std::string find(const std::string &key) const override;
	void prepare();

protected:
	virtual Poco::AutoPtr<Poco::Util::AbstractConfiguration> loadConfig(
			const std::string &path) = 0;

protected:
	Poco::AutoPtr<Poco::Util::LayeredConfiguration> m_config;
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_view;
	std::string m_database;
};

}
