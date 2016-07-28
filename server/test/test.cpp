#include <iostream>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace CppUnit;

int main(int argc, char **argv)
{
	Test *suite = TestFactoryRegistry::getRegistry().makeTest();
	TextUi::TestRunner runner;
	runner.addTest(suite);

	runner.setOutputter(
		new CompilerOutputter(&runner.result(), cerr));
	bool wasSucessful = runner.run();

	return wasSucessful? 0 : 1;
}
