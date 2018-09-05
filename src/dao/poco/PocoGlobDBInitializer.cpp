#include <Poco/Exception.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/Glob.h>
#include <Poco/Logger.h>
#include <Poco/Message.h>
#include <Poco/StreamCopier.h>

#include "di/Injectable.h"
#include "dao/poco/PocoGlobDBInitializer.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoGlobDBInitializer)
BEEEON_OBJECT_CASTABLE(PocoDBInitializer)
BEEEON_OBJECT_PROPERTY("executor", &PocoGlobDBInitializer::setExecutor)
BEEEON_OBJECT_PROPERTY("baseDir", &PocoGlobDBInitializer::setBaseDir)
BEEEON_OBJECT_PROPERTY("patterns", &PocoGlobDBInitializer::setPatterns)
BEEEON_OBJECT_PROPERTY("preprocessors", &PocoGlobDBInitializer::addPreprocessor)
BEEEON_OBJECT_END(BeeeOn, PocoGlobDBInitializer)

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

PocoGlobDBInitializer::PocoGlobDBInitializer()
{
}

void PocoGlobDBInitializer::setBaseDir(const string &dir)
{
	File file = dir;

	if (file.exists()) {
		if (!file.isDirectory())
			throw InvalidArgumentException("path " + dir + " is not a directory");

		if (!file.canRead())
			throw FileAccessDeniedException("cannot read path " + dir);
	}

	m_baseDir = dir;
	m_baseDir.makeDirectory();

}

void PocoGlobDBInitializer::setPatterns(const list<string> &patterns)
{
	m_patterns = patterns;
}

void PocoGlobDBInitializer::addPreprocessor(Preprocessor::Ptr pp)
{
	m_chain.add(pp);
}

void PocoGlobDBInitializer::initialize(Session &session)
{
	set<string> files;

	for (const auto &pattern : m_patterns) {
		const Path pat(m_baseDir, pattern);
		Glob::glob(pat, files, Glob::GLOB_DOT_SPECIAL);
	}

	if (files.empty()) {
		logger().notice("no script to be executed", __FILE__, __LINE__);
		return;
	}

	logger().information("executing " + to_string(files.size()) + " scripts");

	for (const auto &file : files)
		executeScript(session, file);
}

void PocoGlobDBInitializer::executeScript(Session &session, const string &file) const
{
	logger().information("executing script " + file);

	string script;

	FileInputStream in(file);
	StreamCopier::copyToString(in, script);

	script = m_chain.process(script);

	if (logger().trace()) {
		logger().dump(
			"executing script of size " + to_string(script.size()),
			script.data(),
			script.size(),
			Message::PRIO_TRACE);
	}
	else if (logger().debug()) {
		logger().debug(
			"executing script of size " + to_string(script.size()),
			__FILE__, __LINE__);
	}

	const auto result = execute(session, script);

	logger().notice(
		"executed script " + file + " (result: " + to_string(result) + ")",
		__FILE__, __LINE__);
}
