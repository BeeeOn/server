#include <iostream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>

#include <Poco/Logger.h>
#include <Poco/AutoPtr.h>
#include <Poco/File.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/Util/MapConfiguration.h>
#include <Poco/Util/LoggingConfigurator.h>

#include "cppunit/TapTestProducer.h"

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

	TestResult controller;
	BeeeOn::TapTestProducer tapProducer;
	TestRunner runner;

	controller.addListener(&tapProducer);

	Test *suite = TestFactoryRegistry::getRegistry().makeTest();
	runner.addTest(suite);

	runner.run(controller);

	return tapProducer.wasSuccessful()? 0 : 1;
}
