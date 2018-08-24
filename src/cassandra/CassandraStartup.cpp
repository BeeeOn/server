#include <fstream>
#include <iostream>
#include <string>

#include <Poco/Clock.h>
#include <Poco/Error.h>
#include <Poco/Exception.h>
#include <Poco/File.h>
#include <Poco/FileStream.h>
#include <Poco/Logger.h>
#include <Poco/LogStream.h>
#include <Poco/PipeStream.h>
#include <Poco/Process.h>
#include <Poco/RegularExpression.h>
#include <Poco/StreamCopier.h>
#include <Poco/String.h>
#include <Poco/TemporaryFile.h>
#include <Poco/Thread.h>

#include "di/Injectable.h"
#include "cassandra/CassandraStartup.h"

BEEEON_OBJECT_BEGIN(BeeeOn, CassandraStartup)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("cassandraBin", &CassandraStartup::setCassandraBin)
BEEEON_OBJECT_PROPERTY("host", &CassandraStartup::setHost)
BEEEON_OBJECT_PROPERTY("port", &CassandraStartup::setPort)
BEEEON_OBJECT_PROPERTY("rpcPort", &CassandraStartup::setRPCPort)
BEEEON_OBJECT_PROPERTY("jmxPort", &CassandraStartup::setJMXPort)
BEEEON_OBJECT_PROPERTY("storagePort", &CassandraStartup::setStoragePort)
BEEEON_OBJECT_PROPERTY("clusterDir", &CassandraStartup::setClusterDir)
BEEEON_OBJECT_PROPERTY("clusterName", &CassandraStartup::setClusterName)
BEEEON_OBJECT_PROPERTY("clusterInit", &CassandraStartup::setClusterInit)
BEEEON_OBJECT_PROPERTY("stopTimeout", &CassandraStartup::setStopTimeout)
BEEEON_OBJECT_HOOK("done", &CassandraStartup::initCluster)
BEEEON_OBJECT_END(BeeeOn, CassandraStartup)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

const string YAML_FILE = "cassandra.yaml";
const string JMX_PASSWORD_FILE = "jmxremote.password";
const string PID_FILE = "pidfile";

CassandraStartup::CassandraStartup():
	m_cassandraBin("cassandra"),
	m_host("127.0.0.1"),
	m_port(59042),
	m_rpcPort(59160),
	m_jmxPort(57199),
	m_storagePort(7000),
	m_clusterDir(TemporaryFile::tempName(), "cassandra"),
	m_clusterName("Test Cluster"),
	m_clusterInit(false),
	m_stopTimeout(2 * Timespan::SECONDS),
	m_poutGrabber(*this, &CassandraStartup::grabPout)
{
}

CassandraStartup::~CassandraStartup()
{
	try {
		deleteCluster();
	}
	BEEEON_CATCH_CHAIN(logger())
}

void CassandraStartup::setCassandraBin(const string &file)
{
	m_cassandraBin = file;
}

void CassandraStartup::setHost(const string &host)
{
	m_host = host;
}

void CassandraStartup::setPort(const int port)
{
	if (port < 0)
		throw InvalidArgumentException("port must be non-negative");

	m_port = port;
}

void CassandraStartup::setRPCPort(const int port)
{
	if (port < 0)
		throw InvalidArgumentException("rpcPort must be non-negative");

	m_rpcPort = port;
}

void CassandraStartup::setJMXPort(const int port)
{
	if (port < 0)
		throw InvalidArgumentException("jmxPort must be non-negative");

	m_jmxPort = port;
}

void CassandraStartup::setStoragePort(const int port)
{
	if (port < 0)
		throw InvalidArgumentException("storagePort must be non-negative");

	m_storagePort = port;
}

void CassandraStartup::setClusterDir(const string &dir)
{
	m_clusterDir = dir;
}

void CassandraStartup::setClusterName(const string &name)
{
	m_clusterName = name;
}

void CassandraStartup::setClusterInit(bool init)
{
	m_clusterInit = init;
}

void CassandraStartup::setStopTimeout(const Timespan &timeout)
{
	if (timeout < 0)
		throw InvalidArgumentException("stopTimeout must be non-negative");

	m_stopTimeout = timeout;
}

void CassandraStartup::initCluster()
{
	if (!m_clusterInit)
		return;

	File dir(m_clusterDir);
	dir.createDirectories();

	const Path yaml(m_clusterDir, YAML_FILE);
	ofstream config(yaml.toString());
	dumpConfigFile(config);

	logger().notice("created the cassandra cluster at " + m_clusterDir.toString());
}

void CassandraStartup::deleteCluster()
{
	if (!m_clusterInit)
		return;

	logger().notice("deleting the cassandra cluster at " + m_clusterDir.toString());

	File dir(m_clusterDir);

	try {
		dir.remove(true);
	}
	BEEEON_CATCH_CHAIN(logger())
}

void CassandraStartup::start()
{
	logger().notice("starting cassandra...");

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

	const Path yaml(m_clusterDir, YAML_FILE);
	const Path logDir(m_clusterDir, "log");
	const Path jmxPassword(m_clusterDir, JMX_PASSWORD_FILE);

	Process::Args args;
	args.emplace_back("-f");
	args.emplace_back("-Dcassandra.config=file:///" + yaml.toString());
	args.emplace_back("-Dcassandra.logdir=" + logDir.toString());
	args.emplace_back("-Dcassandra.jmx.local.port=" + to_string(m_jmxPort));
	args.emplace_back("-Dcom.sun.management.jmxremote.password.file=" + jmxPassword.toString());
	args.emplace_back("-Dcom.sun.management.jmxremote.authenticate=true");

	Pipe pout;

	m_handle = new ProcessHandle(Process::launch(
	        m_cassandraBin, args, m_clusterDir.toString(), nullptr, &pout, nullptr));

	if (setpgid(m_handle->id(), 0) < 0)
	        logger().error(Error::getMessage(Error::last()), __FILE__, __LINE__);

	m_pout = new Pipe(pout);
	m_poutThread.start(m_poutGrabber);

	logger().notice(
		"cassandra pid is " + to_string(m_handle->id()),
		__FILE__, __LINE__);
}

void CassandraStartup::stop()
{
	logger().information("stopping cassandra...");

	FastMutex::ScopedLock guard(m_lock, m_stopTimeout.totalMilliseconds());

	if (logger().debug()) {
		logger().debug(
			"requesting stop of cassandra " + to_string(m_handle->id()),
			__FILE__, __LINE__);
	}

	try {
		Process::requestTermination(m_handle->id());
		m_handle->wait();
	}
	catch (const NotFoundException &e) {
		logger().log(e, __FILE__, __LINE__);
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}

	m_handle = nullptr;
	m_pout = nullptr;
}

void CassandraStartup::grabPout()
{
	const auto pid = m_handle->id();

	if (logger().debug()) {
		logger().debug(
			"grab stdout of cassandra " + to_string(pid),
			__FILE__, __LINE__);
	}

	while (!m_pout.isNull()) {
		Pipe pipe(*m_pout);
		grab(pipe, pid);
	}
}

static Message::Priority preprocess(string &line)
{
	static RegularExpression labelExpr("(INFO|DEBUG|WARN|ERROR|TRACE|FATAL)");
	Message::Priority prio = Message::PRIO_INFORMATION;

	RegularExpression::MatchVec m;
	if (labelExpr.match(line, 0, m)) {
		const auto label = line.substr(m[1].offset, m[1].length);

		if (label == "INFO")
			prio = Message::PRIO_INFORMATION;
		else if (label == "DEBUG")
			prio = Message::PRIO_DEBUG;
		else if (label == "WARN")
			prio = Message::PRIO_WARNING;
		else if (label == "ERROR")
			prio = Message::PRIO_ERROR;
		else if (label == "TRACE")
			prio = Message::PRIO_TRACE;
		else if (label == "FATAL")
			prio = Message::PRIO_FATAL;

		line = line.substr(m[0].offset + m[0].length);
		trimInPlace(line);
	}

	return prio;
}

void CassandraStartup::grab(Pipe pipe, Process::PID pid) const
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

Process::PID CassandraStartup::readPID()
{
	const Path pidPath(m_clusterDir, PID_FILE);
	FileInputStream pidFile(pidPath.toString());

	int pid = -1;
	pidFile >> pid;
	pidFile.close();

	return pid;
}

static void dumpDataDir(Path clusterDir, ostream &config)
{
	File data(Path(clusterDir, Path("data")));
	data.createDirectories();

	config << "data_file_directories:" << endl;
	config << "    - " << data.path() << endl;
}

static void dumbSeedProvider(string host, ostream &config)
{
	config << "seed_provider:" << endl;
	config << "    - class_name: org.apache.cassandra.locator.SimpleSeedProvider" << endl;
	config << "      parameters:" << endl;
	config << "          - seeds: \"" << host <<  "\"" << endl;
}

void CassandraStartup::dumpConfigFile(ostream &config) const
{
	if (logger().debug()) {
		logger().debug(
			"creating configuration files...",
			__FILE__, __LINE__);
	}

	File hints(Path(m_clusterDir, Path("hints")));
	hints.createDirectories();

	File commitLog(Path(m_clusterDir, Path("commitlog")));
	commitLog.createDirectories();

	File cache(Path(m_clusterDir, Path("saved_caches")));
	cache.createDirectories();

	map<string, string> simpleEntries = {
		{"cluster_name", "\'" + m_clusterName +  "\'"},
		{"hints_directory", hints.path()},
		{"partitioner", "org.apache.cassandra.dht.Murmur3Partitioner"},
		{"commitlog_directory", commitLog.path()},
		{"saved_caches_directory", cache.path()},
		{"commitlog_sync", "periodic"},
		{"commitlog_sync_period_in_ms", "10000"},
		{"commitlog_segment_size_in_mb", "32"},
		{"listen_address", m_host},
		{"start_native_transport", "true"},
		{"native_transport_port", to_string(m_port)},
		{"start_rpc", "false"},
		{"rpc_address", m_host},
		{"rpc_port", to_string(m_rpcPort)},
		{"endpoint_snitch", "SimpleSnitch"},
		{"storage_port", to_string(m_storagePort)},
	};

	for (const auto pair : simpleEntries)
		config << pair.first << ": " << pair.second << endl;

	dumpDataDir(m_clusterDir, config);
	dumbSeedProvider(m_host, config);
}
