/* 
 * File:   SocketServer.h
 * Author: pavel
 *
 * Created on 31. březen 2015, 22:49
 */

#ifndef SOCKETSERVER_H
#define	SOCKETSERVER_H

class SocketServer {
public:
    SocketServer();
    SocketServer(const SocketServer& orig);
    virtual ~SocketServer();
    int start();
private:

};

#endif	/* SOCKETSERVER_H */

