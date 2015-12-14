#ifndef SOCKETSERVER_H
#define	SOCKETSERVER_H

class SocketServer {
public:
    SocketServer();
    SocketServer(const SocketServer& orig);
    virtual ~SocketServer();
    int start(int port);
private:
    int _port;
};

#endif	/* SOCKETSERVER_H */

