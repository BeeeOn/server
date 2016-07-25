#ifndef SERVER_H
#define SERVER_H

namespace BeeeOn {

/**
 * Abstract class representing a server. The server can be
 * started and then it loops until the Control instance
 * asks it for exit.
 */
class Server {
public:
	virtual ~Server() {}

	virtual void start() = 0;
	virtual void stop() = 0;
};

class NullServer : public Server {
public:
	void start() {}
	void stop() {}
};

}

#endif
