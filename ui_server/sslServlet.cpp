
#include "sslServlet.h"

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
void Servlet(SSL* ssl ,std::function<string(char*)> resolveFunc) { 
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
        read:            
                    if (!rc)
                        rc = (char*)malloc (readSize * sizeof (char) + 1);
                    else
                        rc = (char*)realloc (rc, (count + 1) * readSize * sizeof (char) + 1);
                    
                    received = SSL_read(ssl, buf, sizeof(buf)-1); /* get request */
                    buf[received] = '\0';
                    
                    Logger::getInstance(Logger::DEBUG3)<<"ssl read ("<<received<<")result:"<< buf <<endl;
                    
                    if(received > 0){
                        strcat (rc, buf);
                    }else{
                            int sslReadErr = SSL_get_error(ssl,received);
                            break;
                    }
                    if(count > 5){
                        Logger::getInstance(Logger::ERROR)<<"ssl incoming data are too much big"<<endl;
                        break;
                    }
                    if(buf[received-1] == '>' && received < sizeof(buf))
                        break;
                    
                    count++;
                }
                 Logger::getInstance(Logger::DEBUG3)<<"received"<< received<<"count"<<count <<endl;
                if ( received > 0  || count > 1) {
                        std::string replyString = resolveFunc(rc);
#ifdef DEBUG
                        printf("Client msg: \"%s\"\n", buf);
#endif
                        //sprintf(reply, response, buf);   /* construct reply */
                        free(rc);
                        SSL_write(ssl, replyString.c_str(), replyString.length() ); /* send reply */
                        count =0;
                        rc=NULL;
                        goto read;
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
/*
void handleNewSSLconnection(SSL *ssl){
    Servlet(ssl);         
}*/