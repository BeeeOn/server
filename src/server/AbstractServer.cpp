#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "server/AbstractServer.h"

using namespace std;
using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;

AbstractServer::AbstractServer():
	m_bind("127.0.0.1:0"),
	m_backlog(64)
{
}

void AbstractServer::setName(const string &name)
{
	m_name = name;
}

string AbstractServer::name() const
{
	return m_name;
}

void AbstractServer::setHost(const string &host)
{
	m_bind = SocketAddress(host, m_bind.port());
}

void AbstractServer::setPort(int port)
{
	if (port < 0) {
		throw InvalidArgumentException(
				"invalid port number: " + to_string(port));
	}

	m_bind = SocketAddress(m_bind.host(), port);
}

SocketAddress AbstractServer::bindAddress() const
{
	return m_bind;
}

void AbstractServer::setBacklog(int backlog)
{
	if (backlog < 0)
		throw InvalidArgumentException("backlog must be non-negative");

	m_backlog = backlog;
}

int AbstractServer::backlog() const
{
	return m_backlog;
}

void AbstractServer::setEventsExecutor(AsyncExecutor::Ptr executor)
{
	m_eventSource.setAsyncExecutor(executor);
}

void AbstractServer::registerListener(ServerListener::Ptr listener)
{
	m_eventSource.addListener(listener);
}

void AbstractServer::beforeStart()
{
	if (logger().information()) {
		logger().information(
			"starting server " + m_name + " at " + m_bind.toString(),
			__FILE__, __LINE__);
	}
}

void AbstractServer::afterStart()
{
	if (logger().notice()) {
		logger().notice(
			"server " + m_name + " at " + m_bind.toString() + " has started",
			__FILE__, __LINE__);
	}

	const ServerEvent e = {m_bind.toString(), m_name};
	m_eventSource.fireEvent(e, &ServerListener::onUp);
}

void AbstractServer::start()
{
	beforeStart();
	doStart();
	afterStart();
}

void AbstractServer::beforeStop()
{
	if (logger().information()) {
		logger().information(
			"stopping server " + m_name + " at " + m_bind.toString(),
			__FILE__, __LINE__);
	}
}

void AbstractServer::afterStop()
{
	if (logger().notice()) {
		logger().notice(
			"server " + m_name + " at " + m_bind.toString() + " has been stopped",
			__FILE__, __LINE__);
	}

	const ServerEvent e = {m_bind.toString(), m_name};
	m_eventSource.fireEvent(e, &ServerListener::onDown);
}

void AbstractServer::stop()
{
	beforeStop();
	doStop();
	afterStop();
}
