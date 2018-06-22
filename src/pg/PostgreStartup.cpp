#include <Poco/Clock.h>
#include <Poco/Error.h>
#include <Poco/Exception.h>
#include <Poco/File.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/PipeStream.h>
#include <Poco/RegularExpression.h>
#include <Poco/StreamCopier.h>
#include <Poco/String.h>
#include <Poco/TemporaryFile.h>
#include <Poco/Thread.h>

#include "di/Injectable.h"
#include "pg/PostgreStartup.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PostgreStartup)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("postgresBin", &PostgreStartup::setPostgresBin)
BEEEON_OBJECT_PROPERTY("initdbBin", &PostgreStartup::setInitdbBin)
BEEEON_OBJECT_PROPERTY("host", &PostgreStartup::setHost)
BEEEON_OBJECT_PROPERTY("port", &PostgreStartup::setPort)
BEEEON_OBJECT_PROPERTY("clusterDir", &PostgreStartup::setClusterDir)
BEEEON_OBJECT_PROPERTY("clusterInit", &PostgreStartup::setClusterInit)
BEEEON_OBJECT_PROPERTY("stopTimeout", &PostgreStartup::setStopTimeout)
BEEEON_OBJECT_HOOK("done", &PostgreStartup::initdb)
BEEEON_OBJECT_END(BeeeOn, PostgreStartup)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

PostgreStartup::PostgreStartup():
	m_postgresBin("postgres"),
	m_initdbBin("initdb"),
	m_host("127.0.0.1"),
	m_port(54320),
	m_clusterDir(TemporaryFile::tempName()),
	m_clusterInit(false),
	m_stopTimeout(2 * Timespan::SECONDS),
	m_perrGrabber(*this, &PostgreStartup::grabPerr)
{
}

PostgreStartup::~PostgreStartup()
{
	try {
		deletedb();
	}
	BEEEON_CATCH_CHAIN(logger())
}

static void checkExecutable(const string &file)
{
	File bin(file);

	if (!bin.exists())
		throw InvalidArgumentException("no such executable file: " + file);
	if (!bin.canExecute())
		throw InvalidArgumentException("cannot execute file: " + file);
}

void PostgreStartup::setPostgresBin(const string &file)
{
	checkExecutable(file);
	m_postgresBin = file;
}

void PostgreStartup::setInitdbBin(const string &file)
{
	checkExecutable(file);
	m_initdbBin = file;
}

void PostgreStartup::setHost(const string &host)
{
	m_host = host;
}

void PostgreStartup::setPort(const int port)
{
	if (port < 0)
		throw InvalidArgumentException("port must be non-negative");

	m_port = port;
}

void PostgreStartup::setClusterDir(const string &dir)
{
	m_clusterDir = dir;
}

void PostgreStartup::setClusterInit(bool init)
{
	m_clusterInit = init;
}

void PostgreStartup::setStopTimeout(const Timespan &timeout)
{
	if (timeout < 0)
		throw InvalidArgumentException("stopTimeout must be non-negative");

	m_stopTimeout = timeout;
}

void PostgreStartup::initdb()
{
	if (!m_clusterInit)
		return;

	File dir(m_clusterDir);
	dir.createDirectories();

	Process::Args args;
	args.emplace_back(m_clusterDir.toString());
	args.emplace_back("--locale");
	args.emplace_back("C");
	args.emplace_back("--encoding");
	args.emplace_back("UTF8");
	args.emplace_back("--auth-host=trust");
	args.emplace_back("--auth-local=trust");

	logger().information(
		"initializing postgres cluster at " + m_clusterDir.toString());

	Pipe perr;
	Pipe pout;
	auto handle = Process::launch(
		m_initdbBin, args, m_clusterDir.toString(), nullptr, &pout, &perr);

	// we can grab the pipes sequentially because there is not much of content
	PipeInputStream err(perr);
	LogStream logErrors(logger(), Message::PRIO_ERROR);
	StreamCopier::copyStream(err, logErrors);

	PipeInputStream out(pout);
	LogStream logInfo(logger(), Message::PRIO_DEBUG);
	StreamCopier::copyStream(out, logInfo);

	const auto result = handle.wait();

	if (result != 0)
		logger().error("initdb exits with code " + to_string(result));
}

void PostgreStartup::deletedb()
{
	if (!m_clusterInit)
		return;

	logger().notice("deleting the postgres cluster at " + m_clusterDir.toString());

	File dir(m_clusterDir);

	try {
		dir.remove(true);
	}
	BEEEON_CATCH_CHAIN(logger())
}

void PostgreStartup::start()
{
	logger().information("starting postgres...");

	FastMutex::ScopedLock guard(m_lock);

	File clusterFile(m_clusterDir);
	if (!clusterFile.exists()) {
		throw IllegalStateException(
			"directory " + m_clusterDir.toString() + " does not exists");
	}
	if (!clusterFile.isDirectory()) {
		throw IllegalStateException(
			"path " + m_clusterDir.toString() + " is not a directory");
	}
	if (!clusterFile.canWrite()) {
		throw IllegalStateException(
			"directory " + m_clusterDir.toString() + " is not writable");
	}

	Process::Args args;
	args.emplace_back("-D");
	args.emplace_back(m_clusterDir.toString());
	args.emplace_back("-h");
	args.emplace_back(m_host);
	args.emplace_back("-p");
	args.emplace_back(to_string(m_port));
	args.emplace_back("--unix_socket_directories=" + m_clusterDir.toString());
	args.emplace_back("--log_destination=stderr");

	Pipe pout;
	Pipe perr;

	m_handle = new ProcessHandle(Process::launch(
		m_postgresBin, args, m_clusterDir.toString(), nullptr, &pout, &perr));

	if (setpgid(m_handle->id(), 0) < 0)
		logger().error(Error::getMessage(Error::last()), __FILE__, __LINE__);

	if (logger().debug()) {
		logger().debug(
			"postgres started as " + to_string(m_handle->id()),
			__FILE__, __LINE__);
	}

	m_perr = new Pipe(perr);
	m_perrThread.start(m_perrGrabber);
}

void PostgreStartup::stop()
{
	logger().information("stopping postgres...");

	FastMutex::ScopedLock guard(m_lock, m_stopTimeout.totalMilliseconds());

	if (m_handle.isNull()) {
		logger().warning("postgres has not been started");
		return;
	}

	if (logger().debug()) {
		logger().debug(
			"requesting stop of postgres " + to_string(m_handle->id()),
			__FILE__, __LINE__);
	}

	int result = 0;

	try {
		Process::requestTermination(m_handle->id());
		result = Process::wait(*m_handle);
	}
	catch (const NotFoundException &e) {
		logger().log(e, __FILE__, __LINE__);
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);

		logger().warning(
			"killing postgres " + to_string(m_handle->id()),
			__FILE__, __LINE__);

		try {
			Process::kill(*m_handle);
			result = Process::wait(*m_handle);
		}
		catch (const NotFoundException &e) {
			logger().log(e, __FILE__, __LINE__);
		}
		BEEEON_CATCH_CHAIN(logger())
	}

	if (result != 0) {
		logger().warning(
			"postgres " + to_string(m_handle->id())
			+ " exits with code " + to_string(result));
	}

	m_handle = nullptr;
	m_perr = nullptr;
}

void PostgreStartup::grabPerr()
{
	const auto pid = m_handle->id();

	if (logger().debug())
		logger().debug("grab stderr of postgres " + to_string(pid));

	while (!m_perr.isNull()) {
		Pipe pipe(*m_perr);
		grab(pipe, pid);
	}
}

static Message::Priority preprocess(string &line)
{
	static RegularExpression labelExpr("(INFO|NOTICE|WARNING|ERROR|LOG|HINT|FATAL|PANIC):");
	Message::Priority prio = Message::PRIO_INFORMATION;

	RegularExpression::MatchVec m;
	if (labelExpr.match(line, 0, m)) {
		const auto label = line.substr(m[1].offset, m[1].length);

		if (label == "INFO")
			prio = Message::PRIO_INFORMATION;
		else if (label == "NOTICE")
			prio = Message::PRIO_NOTICE;
		else if (label == "WARNING")
			prio = Message::PRIO_WARNING;
		else if (label == "ERROR")
			prio = Message::PRIO_ERROR;
		else if (label == "LOG")
			prio = Message::PRIO_INFORMATION;
		else if (label == "HINT")
			prio = Message::PRIO_WARNING;
		else if (label == "FATAL")
			prio = Message::PRIO_FATAL;
		else if (label == "PANIC")
			prio = Message::PRIO_CRITICAL;

		line = line.substr(m[0].offset + m[0].length);
		trimInPlace(line);
	}

	return prio;
}

void PostgreStartup::grab(Pipe pipe, Process::PID pid) const
{
	PipeInputStream in(pipe);
	string line;

	while (getline(in, line)) {
		Message::Priority prio = preprocess(line);

		Message msg(
			logger().name(),
			line,
			prio,
			__FILE__,
			__LINE__
		);

		msg.setPid(pid);
		logger().log(msg);
	}
}
