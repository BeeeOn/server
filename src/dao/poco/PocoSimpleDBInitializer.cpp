#include <Poco/FileStream.h>
#include <Poco/Logger.h>
#include <Poco/StreamCopier.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSimpleDBInitializer.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSimpleDBInitializer)
BEEEON_OBJECT_CASTABLE(PocoDBInitializer)
BEEEON_OBJECT_PROPERTY("script", &PocoSimpleDBInitializer::loadScript)
BEEEON_OBJECT_END(BeeeOn, PocoSimpleDBInitializer)

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

void PocoSimpleDBInitializer::loadScript(const string &path)
{
	if (logger().debug()) {
		logger().debug("loading script from " + path,
				__FILE__, __LINE__);
	}

	FileInputStream in(path);
	StreamCopier::copyToString(in, m_script);

	m_file = path;
}

void PocoSimpleDBInitializer::initialize(Session &session)
{
	if (m_script.empty()) {
		logger().notice("no script to be executed",
				__FILE__, __LINE__);
		return;
	}

	if (logger().trace()) {
		logger().trace("executing script:\n" + m_script,
				__FILE__, __LINE__);
	}

	const auto result = execute(session, m_script);

	logger().notice(
		"executed script "
		+ m_file
		+ " (result: "
		+ to_string(result)
		+ ")",
		__FILE__, __LINE__);
}
