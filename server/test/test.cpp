#include <iostream>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/File.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/Util/MapConfiguration.h>
#include <Poco/Util/LoggingConfigurator.h>

using namespace std;
using namespace CppUnit;
using namespace Poco;
using namespace Poco::Util;

void setupLogger(const std::string &path)
{
	File file(path);
	LoggingConfigurator logConfig;

	if (!file.exists()) {
		AutoPtr<MapConfiguration> config(new MapConfiguration);

		logConfig.configure(config);
		cerr << "failed to locate logging configuration file "
			<< file.path() << endl;
		cerr << "try: ./test-suite <path-to>/logging.ini" << endl;
	}
	else {
		AutoPtr<IniFileConfiguration> config(
				new IniFileConfiguration(file.path()));
		logConfig.configure(config);
	}
}

int main(int argc, char **argv)
{
	setupLogger(argc > 1? argv[1] : "logging.ini");

	Test *suite = TestFactoryRegistry::getRegistry().makeTest();
	TextUi::TestRunner runner;
	runner.addTest(suite);

	runner.setOutputter(
		new CompilerOutputter(&runner.result(), cerr));
	bool wasSucessful = runner.run();

	return wasSucessful? 0 : 1;
}
