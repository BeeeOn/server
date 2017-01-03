#ifndef BEEEON_SQL_LOADER_H
#define BEEEON_SQL_LOADER_H

#include <Poco/AutoPtr.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "di/InjectorTarget.h"

namespace BeeeOn {

class SQLLoader : public AbstractInjectorTarget {
public:
	SQLLoader();
	virtual ~SQLLoader();

	void addSourceFile(const std::string &path);

	void setDatabase(const std::string &name);

	std::string find(const std::string &key) const;

protected:
	void injectionDone() override;

protected:
	Poco::AutoPtr<Poco::Util::LayeredConfiguration> m_config;
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> m_view;
	std::string m_database;
};

}

#endif