#pragma once

#include <Poco/AutoPtr.h>
#include <Poco/Util/LayeredConfiguration.h>
#include <Poco/Util/AbstractConfiguration.h>

#include "dao/AbstractConfigurationQueryLoader.h"

namespace BeeeOn {

class IniFileQueryLoader : public AbstractConfigurationQueryLoader {
public:
	IniFileQueryLoader();

protected:
	Poco::AutoPtr<Poco::Util::AbstractConfiguration> loadConfig(
			const std::string &path) override;
};

}
