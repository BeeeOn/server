#pragma once

#include <string>

#include "dao/poco/PocoDBInitializer.h"
#include "util/Loggable.h"

namespace BeeeOn {

class PocoSimpleDBInitializer : public PocoDBInitializer, Loggable {
public:
	/**
	 * Execute the loaded script.
	 */
	void initialize(Poco::Data::Session &session) override;

	/**
	 * Load script from the given file path.
	 */
	void loadScript(const std::string &path);

private:
	std::string m_file;
	std::string m_script;
};

}
