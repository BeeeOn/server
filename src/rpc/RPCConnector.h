#ifndef BEEEON_RPC_CONNECTOR_H
#define BEEEON_RPC_CONNECTOR_H

#include <iosfwd>

namespace BeeeOn {

class RPCConnector {
public:
	virtual ~RPCConnector();
	/**
	 * Connect the remote service.
	 */
	virtual void open() = 0;

	/**
	 * Return ostream to write to.
	 */
	virtual std::ostream &send() = 0;

	/**
	 * Ensure everything has been sent.
	 */
	virtual void flush() = 0;

	/**
	 * Return istream to read from.
	 */
	 virtual std::istream &receive() = 0;

	/**
	 * Close the connection.
	 */
	virtual void close() = 0;
};

}

#endif
