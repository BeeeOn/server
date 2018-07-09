#include <Poco/Exception.h>
#include <Poco/Nullable.h>
#include <Poco/String.h>

#include <Poco/JSON/Object.h>

#include "di/Injectable.h"
#include "gwmessage/GWNewDeviceRequest.h"
#include "io/SimpleConsole.h"
#include "util/ArgsParser.h"
#include "util/DeviceMatchingTester.h"
#include "util/JsonUtil.h"
#include "util/MultiException.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceMatchingTester)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("deviceInfoProvider", &DeviceMatchingTester::setDeviceProvider)
BEEEON_OBJECT_PROPERTY("cmd", &DeviceMatchingTester::setCommand)
BEEEON_OBJECT_PROPERTY("console", &DeviceMatchingTester::setConsole)
BEEEON_OBJECT_END(BeeeOn, DeviceMatchingTester)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

DeviceMatchingTester::DeviceMatchingTester()
{
}

DeviceMatchingTester::~DeviceMatchingTester()
{
}

void DeviceMatchingTester::setDeviceProvider(DeviceInfoProvider::Ptr provider)
{
	m_provider = provider;
}

void DeviceMatchingTester::main(
	ConsoleSession &session,
	const vector<string> &)
{
	if (!command().empty()) {
		processLine(command(), session);
		return;
	}

	bool stop = false;

	while (!stop && !shouldStop()) {
		if (session.eof())
			break;

		string line = session.readLine();
		trimInPlace(line);

		if (line.empty())
			continue;

		try {
			stop = processLine(line, session);
		}
		catch (const Exception &e) {
			session.print(e.displayText());
		}
	}
}

void DeviceMatchingTester::reportShort(const SharedPtr<DeviceInfo> info, Printable &out)
{
	if (info.isNull())
		throw NotFoundException("no such device info");

	out.print("name:   " + info->name());
	out.print("vendor: " + info->vendor());
	out.print("matchers:");

	for (const auto matcher : info->matchers()) {
		const auto exact = matcher.cast<DeviceInfo::MatchExact>();
		const auto glob = matcher.cast<DeviceInfo::MatchGlob>();

		if (!exact.isNull())
			out.print("  exact: \"" + exact->vendor() + "\" \"" + exact->name() + "\"");
		else if (!glob.isNull())
			out.print("  glob: \"" + glob->vendorPattern() + "\" \"" + glob->namePattern() + "\"");
		else
			out.print("  <unsupported>");
	}
}

void DeviceMatchingTester::reportResult(const SharedPtr<DeviceInfo> info, Printable &out)
{
	out.print("# Result");
	if (info.isNull())
		throw NotFoundException("no such device info");

	out.print("name:   " + info->name());
	out.print("vendor: " + info->vendor());
	out.print("modules:");

	for (const auto &module : *info) {
		out.print("  name:    " + module.name());
		out.print("  kind:    " + module.isControllable() ? "controllable" : "sensor");
		out.print("  type:    " + module.type()->name());
		out.print("  unit:    " + module.type()->unit());
		out.print("  subtype: " + (module.subtype().isNull() ? "N/A" : module.subtype()->name()));
		out.print("  group:   " + (module.group().empty() ? "<none>" : module.group()));
	}
}

bool DeviceMatchingTester::processLine(const string &line, Printable &out)
{
	ArgsParser parser;
	MultiException failures;

	try {
		parser.parse(line);
	}
	catch (const Exception &e) {
		failures.caught(e);
	}

	if (failures.empty()) {
		if (parser[0] == "help") {
			out.print("commands:");
			out.print("* match <vendor> <product-name>");
			out.print("* list");
			out.print("* exit|quit");
			out.print("* any other string is parsed as JSON GWNewDeviceRequest message");
			return false;
		}
		else if (parser[0] == "exit" || parser[0] == "quit") {
			return true;
		}
		else if (parser[0] == "match") {
			if (parser.count() < 3)
				throw InvalidArgumentException("expected: match <vendor> <product-name>");

			out.print("# Request");
			out.print("vendor:  " + parser[1]);
			out.print("product: " + parser[2]);

			SharedPtr<DeviceInfo> info = m_provider->findByNameAndVendor(parser[2], parser[1]);
			reportResult(info, out);
			return false;
		}
		else if (parser[0] == "list") {
			for (const auto info : *m_provider)
				reportShort(info, out);

			return false;
		}
	}

	Object::Ptr json;

	try {
		json = JsonUtil::parse(line);
	} catch (const Exception &e) {
		failures.caught(e);
		failures.rethrow();
	}

	const GWNewDeviceRequest request(json);

	out.print("# Request");
	out.print("vendor:  " + request.vendor());
	out.print("product: " + request.productName());

	SharedPtr<DeviceInfo> info = m_provider->findByNameAndVendor(
		request.productName(), request.vendor());
	reportResult(info, out);

	return false;
}
