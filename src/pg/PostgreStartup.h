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
 * @brief PostgreStartup can start a PostgreSQL instance and initialize
 * a new clean database named 'postgres'. This is useful especially for
 * testing to avoid any complexity when creating a private temporary
 * databases.
 */
class PostgreStartup : public StoppableLoop, Loggable {
public:
	PostgreStartup();
	~PostgreStartup();

	/**
	 * @brief Configure path to the 'postgres' binary.
	 */
	void setPostgresBin(const std::string &file);

	/**
	 * @brief Configure path to the 'initdb' binary.
	 */
	void setInitdbBin(const std::string &file);

	/**
	 * @brief Configure host to listen on (default '127.0.0.1').
	 */
	void setHost(const std::string &host);

	/**
	 * @brief Configure port to listen on (default 54320).
	 */
	void setPort(const int port);

	/**
	 * @brief Configure directory with the cluster. There might
	 * be an existing database or a temporary one might be created.
	 */
	void setClusterDir(const std::string &dir);

	/**
	 * @brief Configure PostgreStartup to initialize the cluster
	 * by initdb. This might destroy any databases in an existing
	 * cluster. If enabled, the cluster is deleted automatically
	 * on clean up. Otherwise, the cluster is left untouched there.
	 */
	void setClusterInit(bool init);

	/**
	 * @brief Configure timeout used while waiting for the 'postgres'
	 * process to exit after termination request.
	 */
	void setStopTimeout(const Poco::Timespan &timeout);

	/**
	 * @brief If clusterInit is true, this method calls initdb to
	 * initialize a temporary database cluster.
	 */
	void initdb();

	/**
	 * @brief If clusterInit is true, this method removes the temporary
	 * database cluster on hook 'cleanup'.
	 */
	void deletedb();

	/**
	 * @brief Start the 'postgres' program on the configured cluster.
	 */
	void start() override;

	/**
	 * @brief Stop the 'postgres' process.
	 */
	void stop() override;

protected:
	/**
	 * @brief Grab process stderr stream and log it via the standard logger.
	 */
	void grabPerr();
	void grab(
		Poco::Pipe pipe,
		Poco::Process::PID pid) const;

private:
	std::string m_postgresBin;
	std::string m_initdbBin;
	std::string m_host;
	int m_port;
	Poco::Path m_clusterDir;
	bool m_clusterInit;
	Poco::Timespan m_stopTimeout;
	Poco::SharedPtr<Poco::ProcessHandle> m_handle;
	Poco::SharedPtr<Poco::Pipe> m_perr;
	Poco::Thread m_perrThread;
	Poco::RunnableAdapter<PostgreStartup> m_perrGrabber;
	Poco::FastMutex m_lock;
};

}
