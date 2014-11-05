// Server.cpp : Defines the entry point for the console application.

#include "communication.h"
#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>
#include <iostream>
#include "Logger.h"
#include "DBConnector.h"
#include "gTokenChecker.h"
#include "ComTable.h"
#include "pugixml.hpp"
#include "MsgRightsChecker.h"
#include "ServerException.h"
#include "SocketClient.h"
#include "Config.h"
#include <signal.h>
#include <atomic>

// uncoment if you want print debug reports
//#define DEBUG 1

#define FAIL    -1

#define SERVER_PORT 4565


/**
 * certificate paths
 */
char CertFile[] = "./cert/ant-2.fit.vutbr.cz.crt";
char KeyFile[]  = "./cert/ant-2.fit.vutbr.cz.key";

//using namespace std;

int OpenListener(int port) {
        int sd;
        struct sockaddr_in addr;
        
        sd = socket(PF_INET, SOCK_STREAM, 0);
        bzero(&addr, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        
        if ( ::bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 ) {
                perror("can't bind port");
                abort();
        }
        
        
        if ( listen(sd, 10) != 0 ) {
                perror("Can't configure listening port");
                abort();
        }
        return sd;
}


SSL_CTX* InitServerCTX(void) {
        const SSL_METHOD *method;
        SSL_CTX *ctx;
        
        OpenSSL_add_all_algorithms();  /* load & register all cryptos, etc. */
        SSL_load_error_strings();   /* load all error messages */
        
        //method = TLSv1_1_server_method();  /* create new server-method instance */
        //method = SSLv23_server_method();  /* create new server-method instance */
        //method = SSLv3_server_method();  /* create new server-method instance */
        method = TLSv1_server_method();  /* create new server-method instance */
        
        ctx = SSL_CTX_new(method);   /* create new context from method */
        if ( ctx == NULL ) {
                ERR_print_errors_fp(stderr);
                abort();
        }
        return ctx;
}

void LoadCertificates(SSL_CTX* ctx, char* CertFile, char* KeyFile) {
        //New lines 
        if (SSL_CTX_load_verify_locations(ctx, CertFile, KeyFile) != 1)
                ERR_print_errors_fp(stderr);
        
        if (SSL_CTX_set_default_verify_paths(ctx) != 1)
                ERR_print_errors_fp(stderr);
        //End new lines
        
        /* set the local certificate from CertFile */
        if (SSL_CTX_use_certificate_chain_file(ctx, CertFile) <= 0) {
                ERR_print_errors_fp(stderr);
                abort();
        }
        /* set the private key from KeyFile (may be the same as CertFile) */
        if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0) {
                ERR_print_errors_fp(stderr);
                abort();
        }
        /* verify private key */
        if (!SSL_CTX_check_private_key(ctx)) {
                fprintf(stderr, "Private key does not match the public certificate\n");
                abort();
        }
        
        //New lines - Force the client-side have a certificate
        //SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, NULL);
        //SSL_CTX_set_verify_depth(ctx, 4);
        //End new lines
}

void ShowCerts(SSL* ssl) {   
#ifdef DEBUG
        X509 *cert;
        char *line;
        
        cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
        if ( cert != NULL ) {
                printf("Server certificates:\n");
                line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
                printf("Subject: %s\n", line);
                free(line);
                line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
                printf("Issuer: %s\n", line);
                free(line);
                X509_free(cert);
        }
        else {
                printf("No certificates.\n");
        }
#endif
}

/* Serve the connection -- threadable */
void Servlet(SSL* ssl) { 
        Logger::getInstance(Logger::DEBUG3)<<"servlet start"<<endl;
        
        int fd = SSL_get_fd(ssl);
        struct timeval tv;
        tv.tv_sec = 3;
        tv.tv_usec = 0; 
        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
        SSL_set_fd(ssl, fd);
        
        char buf[1025];
        int sd, received;
        int ret;
        int readSize = 1024;
        char *rc=NULL;
        int count = 0;
        again:
        if ( (ret = SSL_accept(ssl)) != 1 ) {     /* do SSL-protocol accept */
                ERR_print_errors_fp(stderr);
                int ssl_err = SSL_get_error(ssl, ret);
                Logger::getInstance(Logger::DEBUG3)<<"ssl_accept err:"<<ssl_err<<endl;
                /*if( ssl_err == SSL_ERROR_WANT_READ ){
                        Logger::getInstance(Logger::DEBUG3)<<"jump"<<endl;
                        goto again;
                }*/
        }else {
                Logger::getInstance(Logger::DEBUG3)<<"ssl accepted"<<endl;
                // print client certificate
                //ShowCerts(ssl);
                
                while(1){
                    
                    if (!rc)
                        rc = (char*)malloc (readSize * sizeof (char) + 1);
                    else
                        rc = (char*)realloc (rc, (count + 1) * readSize * sizeof (char) + 1);

                    received = SSL_read(ssl, buf, sizeof(buf)-1); /* get request */
                    buf[received] = '\0';
                
                    if(received > 0){
                        strcat (rc, buf);
                    }else{
                            int sslReadErr = SSL_get_error(ssl,received);
                            Logger::getInstance(Logger::DEBUG3)<<"ssl read err"<< sslReadErr <<endl;
                            //if(sslReadErr == SSL_ERROR_WANT_READ)
                            //goto readMore;
                    }
                    if(count > 5){
                        Logger::getInstance(Logger::ERROR)<<"ssl incoming data are too much big"<<endl;
                        break;
                    }
                    if(buf[received-1] == '>' && received < sizeof(buf))
                        break;
                    
                    count++;
                }
                
                if ( received > 0  || count > 0) {
                        std::string replyString = resolveMsg(rc);
#ifdef DEBUG
                        printf("Client msg: \"%s\"\n", buf);
#endif
                        //sprintf(reply, response, buf);   /* construct reply */
                        free(rc);
                        SSL_write(ssl, replyString.c_str(), replyString.length() ); /* send reply */
                }
                else {
                        ERR_print_errors_fp(stderr);
                }
        }
         Logger::getInstance(Logger::DEBUG3)<<"ssl com done"<<endl;
        sd = SSL_get_fd(ssl);       /* get socket connection */
        SSL_free(ssl);         /* release SSL state */
        close(sd);          /* close connection */
        Logger::getInstance(Logger::DEBUG3)<<"servlet done"<<endl;
}

void handleNewSSLconnection(SSL *ssl){
    Servlet(ssl);         /* service connection */
}

bool serverStop;
int serverPort;

void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        serverStop = true;
        Logger::getInstance(Logger::FATAL) << "SIGINT catched"<<endl;
        SocketClient sc(serverPort);
        sc.write("end");
    }
}


int main(int argc, char** argv)
{   
    if(argc>=2){
        try{
            Config::getInstance().loadXml(argv[1]);
        }catch(string & e){
            cerr <<e<<endl;
            return 1;
        }
    }
        serverPort = Config::getInstance().getServerPort();

    if (signal(SIGINT, sig_handler) == SIG_ERR)
    {
            Logger::getInstance(Logger::FATAL) << "unable to catch signal SIGINT "<<endl;
    }

        Logger::getInstance().setVerbosityThreshold( Config::getInstance().getVerbosity() );
        
        Logger::getInstance(Logger::FATAL) << "start with port"<<serverPort <<endl;
        ComTable::getInstance().startComTableCleaner(Config::getInstance().getComTableSleepPeriodMs(), Config::getInstance().getComTableMaxInactivityMs() );
        try{                        
            DBConnector::getInstance().setConnectionStringAndOpenSessions( Config::getInstance().getDBConnectionString() , Config::getInstance().getDBSessionsNumber());
        }
        catch (soci::soci_error const & e)
        {
            Logger::getInstance(Logger::ERROR) << "DB error (soci): " << e.what() << endl;
            
        }
        /* device d;
        d.id = "53.54.55.56";
        d.type="0x00";
        DBConnector::getInstance().getDeviceLog((string)"51966", d,(string)"2014-08-05 00:00:00",(string)"2014-08-12 11:23:50",(string)"avg",(string)"3600");
        return 0;*/
                
        
        resolveMsg( "<com ver=\"2.2\" sid=\"0\" state=\"signin\" email=\"user2@gmail.com\" gt=\"1\"  loc=\"cs\" />");
        resolveMsg( "<com ver=\"2.2\" sid=\"1\" state=\"getalldevs\" aid=\"20\" />");
        resolveMsg( "<com ver=\"2.2\" sid=\"1\" state=\"getdevs\" ><adapter id=\"20\"><dev id=\"0.0.20.1\"><part type=\"1\" /></dev><dev id=\"0.0.10.2\"><part type=\"1\" /></dev></adapter></com>");
        resolveMsg( "<com ver=\"2.2\" sid=\"1\" state=\"getlog\" from=\"1377684610\" ftype=\"avg\" interval=\"0\" aid=\"21\" did=\"0.0.21.1\" dtype=\"1\" />" );
       resolveMsg( "<com ver=\"2.2\" sid=\"1\" state=\"getnewdevs\" aid=\"10\"  />" );

        
        SSL_CTX *ctx;
        int server;
        atomic<int>* threadCounter = new atomic<int>(0);
        
        SSL_library_init();
        
        ctx = InitServerCTX();        /* initialize SSL */
        LoadCertificates(ctx, CertFile, KeyFile); /* load certs */
        server = OpenListener(serverPort);    /* create server socket */
        while (!serverStop) {
            struct sockaddr_in addr;
            socklen_t len = sizeof(addr);
            
            int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
            Logger::getInstance(Logger::DEBUG3)<<"socket accepted"<<endl;
#ifdef DEBUG
            printf("Connection: %s:%d\n",inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
#endif
            SSL *ssl;
            ssl = SSL_new(ctx);              /* get new SSL state with context */
            SSL_set_fd(ssl, client);      /* set connection socket to SSL state */
            if(1){
                Servlet(ssl);
            }else{
                while(*threadCounter > Config::getInstance().getServerThreadsNumber()) 
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));

                Logger::getInstance(Logger::DEBUG2)<<"threads c.:"<<threadCounter<<endl;
                Logger::getInstance(Logger::DEBUG2)<<"new thread started"<<endl;
                thread *t =  new thread( [ssl, threadCounter](){
                    (*threadCounter)++;
                    Servlet(ssl);
                    (*threadCounter)--;
                });
            }
        }
        
        close(server);          /* close server socket */
        SSL_CTX_free(ctx);         /* release context */
        Logger::getInstance(Logger::FATAL) << "Server main thread exit\n";
        return 0;
}