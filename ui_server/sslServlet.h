/* 
 * File:   sslServlet.h
 * Author: pavel
 *
 * Created on 14. listopad 2014, 17:30
 */

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

using namespace std;
int OpenListener(int port);

void Servlet(SSL* ssl ,std::function<string(char*)> resolveFunc);

SSL_CTX* InitServerCTX(void);
void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile) ;
void ShowCerts(SSL* ssl);
#endif	/* SSLSERVLET_H */

