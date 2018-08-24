#pragma once

#include <iosfwd>
#include <string>

#include <Poco/Message.h>
#include <Poco/Mutex.h>
#include <Poco/Path.h>
#include <Poco/Pipe.h>
#include <Poco/Process.h>
#include <Poco/RunnableAdapter.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>
#include <Poco/Thread.h>

#include "loop/StoppableLoop.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief CassandraStartup can start a Cassandra instance and initialize
 * a new clean database named 'cassandra'. This is useful especially for
 * testing to avoid any complexity when creating a private temporary
 * databases.
 */
class CassandraStartup : public StoppableLoop, Loggable {
public:
	CassandraStartup();
	~CassandraStartup();

	/**
	 * @brief Configure path to the 'cassandra' starting script.
	 */
	void setCassandraBin(const std::string &file);

	/**
	 * @brief Configure host to listen on (default '127.0.0.1').
	 */
	void setHost(const std::string &host);

	/**
	 * @brief Configure port to listen on (default 59042).
	 */
	void setPort(const int port);

	/**
	 * @brief Configure RPC port (default 59160).
	 */
	void setRPCPort(const int port);

	/**
	 * @brief Configure JMX port (default 57199).
	 */
	void setJMXPort(const int port);

	/**
	 * @brief Configure port for inter-node communication.
	 */
	void setStoragePort(const int port);

	/**
	 * @brief Configure directory with the cluster. There might
	 * be an existing database or a temporary one might be created.
	 */
	void setClusterDir(const std::string &dir);

	/**
	 * @brief Name of the cluster (default is Test Cluster).
	 */
	void setClusterName(const std::string &name);

	/**
	 * @brief Configure CassandraStartup to initialize the cluster
	 * by initCluster. This might destroy any databases in an existing
	 * cluster. If enabled, the cluster is deleted automatically
	 * on clean up. Otherwise, the cluster is left untouched there.
	 */
	void setClusterInit(bool init);

	/**
	 * @brief Configure timeout used while waiting for the 'cassandra'
	 * process to exit after termination request.
	 */
	void setStopTimeout(const Poco::Timespan &timeout);

	/**
	 * @brief If clusterInit is true, this method calls initCluster to
	 * initialize a temporary database cluster.
	 */
	void initCluster();

	/**
	 * @brief If clusterInit is true, this method removes the temporary
	 * database cluster on hook 'cleanup'.
	 */
	void deleteCluster();

	/**
	 * @brief Start the 'cassandra' program on the configured cluster.
	 */
	void start() override;

	/**
	 * @brief Stop the 'cassandra' process.
	 */
	void stop() override;

protected:
	/**
	 * @brief Grab process stdout stream and log it via the standard logger.
	 */
	void grabPout();
	void grab(
		Poco::Pipe pipe,
		Poco::Process::PID pid) const;

	/**
	 * @brief returns PID of 'cassandra' process.
	 */
	int readPID();

	/*
	 * @brief creates config (.yaml) file in m_clusterDir location.
	 * The configuration file is called cassandra.yaml.
	 * This method also creates necessary folders:
	 * - hints/
	 * - commitlog/
	 * - saved_cache/
	 * - data/ - Cassandra should store data on disk here
	 *   and cassandra will spread data evenly across them
	 */
	void dumpConfigFile(std::ostream &config) const;

private:
	std::string m_cassandraBin;
	std::string m_host;
	int m_port;
	int m_rpcPort;
	int m_jmxPort;
	int m_storagePort;
	Poco::Path m_clusterDir;
	std::string m_clusterName;
	bool m_clusterInit;
	Poco::Timespan m_stopTimeout;
	Poco::SharedPtr<Poco::ProcessHandle> m_handle;
	Poco::SharedPtr<Poco::Pipe> m_pout;
	Poco::Thread m_poutThread;
	Poco::RunnableAdapter<CassandraStartup> m_poutGrabber;
	Poco::FastMutex m_lock;
};

}
