#pragma once

#include <string>

#include <Poco/Net/SocketAddress.h>

#include "loop/StoppableLoop.h"
#include "server/ServerListener.h"
#include "util/EventSource.h"
#include "util/Loggable.h"

namespace BeeeOn {

class AbstractServer :
	public StoppableLoop,
	protected virtual Loggable {
public:
	AbstractServer();

	void setName(const std::string &name);
	std::string name() const;

	void setHost(const std::string &host);
	void setPort(int port);
	void setBacklog(int backlog);
	int backlog() const;

	void setEventsExecutor(AsyncExecutor::Ptr executor);
	void registerListener(ServerListener::Ptr listener);

	void start() override;
	void stop() override;

protected:
	void beforeStart();
	virtual void doStart() = 0;
	void afterStart();

	void beforeStop();
	virtual void doStop() = 0;
	void afterStop();

	Poco::Net::SocketAddress bindAddress() const;

private:
	std::string m_name;
	Poco::Net::SocketAddress m_bind;
	int m_backlog;
	EventSource<ServerListener> m_eventSource;
};

}
