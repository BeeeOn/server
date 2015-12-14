#ifndef SSLSERVLET_H
#define	SSLSERVLET_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

#include <string>
#include <iostream>
#include <functional>

#include "../ui_logger/Logger.h"
#include "RequestHandlerFactory.h"

int OpenListener(int port);

std::string resolveRequest(RequestHandlerFactory & requesthandlerFac, std::string request);

void Servlet(SSL* ssl, RequestHandlerFactory & requesthandlerFac);

SSL_CTX* InitServerCTX(void);
void LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile) ;
void ShowCerts(SSL* ssl);
#endif	/* SSLSERVLET_H */

