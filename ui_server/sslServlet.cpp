
#include "sslServlet.h"
#include "communication.h"
#include <errno.h> 

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
// http://stackoverflow.com/questions/13686398/ssl-read-failing-with-ssl-error-syscall-error
// http://jmarshall.com/stuff/handling-nbio-errors-in-openssl.html
void logErrors(SSL* ssl, int returnValue){
    Logger::debug3()<<"SSLerr loging, err occuerd, return value =  " << returnValue << endl;
    ERR_print_errors_fp(stderr);
    int ssl_err = SSL_get_error(ssl, returnValue);
    int err = ERR_get_error();
    Logger::error()<<"ssl err: "<<ssl_err << " get_Err: " << err << " err_string " << ERR_error_string(ssl_err, NULL) << endl;
    if(ssl_err != 0){
        Logger::debug3() << "errno: " << errno << endl << "perror: " << strerror(errno) << endl;
    }
}

bool endsWith (char* base, char* str) {
    int blen = strlen(base);
    int slen = strlen(str);
    return (blen >= slen) && (0 == strcmp(base + blen - slen, str));
}


int readMsgFromSSL(SSL* ssl, char** rc){
    
    
    char buf[1025];
    //bzero(buf, 1025);
    int readSize = 1024;
    int received = 0;  
    int readCount = 0;
    do{ 
        if (!(*rc)){
            *rc = (char*)malloc (readSize * sizeof (char) + 1);
            bzero(*rc, readSize * sizeof (char) + 1);
            //Logger::getInstance(Logger::DEBUG3)<<"rc len" <<endl<< strlen(*rc)<<endl;
        }else{
            *rc = (char*)realloc (*rc, (readCount + 1) * readSize * sizeof (char) + 1);
        }
        received = SSL_read(ssl, buf, sizeof(buf)-1); // get request 
        if(received >= 0){
            buf[received] = '\0';
            Logger::getInstance(Logger::DEBUG3)<<"ssl read ("<<received<<")result:"<< buf <<endl;
            
            char *const strBuf = (char*)malloc(strlen(buf) + 1);
            
            memcpy( (void*)strBuf,(void*)buf, strlen(buf)+1);
            //free(strBuf);
            strcat (*rc, strBuf);
            //const char src[50] = "http://www.tutorialspoint.com";
            //char dest[50];

            //printf("Before memcpy dest = %s\n", dest);
        }else if(received < 0){
                logErrors(ssl, received);   
                break;
        }
        if(readCount > 5){
            Logger::getInstance(Logger::ERROR)<<"ssl incoming data are too big"<<endl;
            break;
        }

        readCount++;
        
    }while( !endsWith(*rc, (char*)"</com>") );
    
    (*rc)[strlen(*rc)] = '\0';
    Logger::getInstance(Logger::DEBUG3)<<"ssl whole read ("<<received<< " vs "<< strlen(*rc) <<")result:"<< *rc <<endl;
    
    
    return received;
}

/* Serve the connection -- threadable */
void Servlet(SSL* ssl ,std::function<string(char*)> resolveFunc) { 
        Logger::getInstance(Logger::DEBUG3)<<"servlet start"<<endl;
        
        int fd = SSL_get_fd(ssl);
        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0; 
        setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv,sizeof(struct timeval));
        SSL_set_fd(ssl, fd);
        
        int sd, received;
        int ret;
        char *rc=NULL;
        int burstMsgCount = 1;
        
        //http://comments.gmane.org/gmane.comp.encryption.openssl.user/49443
        if ( (ret = SSL_accept(ssl)) != 1 ) {     // do SSL-protocol accept 
            Logger::getInstance(Logger::DEBUG3)<<"ssl accept FAIL"<<endl;
            logErrors(ssl, ret);                
        }else {
                Logger::getInstance(Logger::DEBUG3)<<"ssl accepted"<<endl;
                // print client certificate
                //ShowCerts(ssl);
           
           do{
               
                rc=NULL;
                received = readMsgFromSSL(ssl, &rc);
                
                Logger::getInstance(Logger::DEBUG3)<<"received"<< received << " : " <<rc<< endl;
                if ( received > 0) {
                    Logger::getInstance(Logger::DEBUG3)<<"Start resolve "<< burstMsgCount++ <<" msg in burst"<<endl;

                    std::string replyString = resolveMsg(rc);
                    
                    //replyString.insert(0, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"); 
                    //Logger::getInstance(Logger::DEBUG3)<<"last chars of reply:"<< replyString.substr(replyString.length()-10, 10)<<"<"<<endl;

                    int writeBytes = SSL_write(ssl, replyString.c_str(), replyString.length() ); // send reply 
                    Logger::getInstance(Logger::DEBUG3)<<"write "<< writeBytes<<endl;
                    if(writeBytes<0){
                        logErrors(ssl, writeBytes);   
                    }
                    //buf[0] = '\0';
                }else{
                    ERR_print_errors_fp(stderr);
                }
            
                if (!rc)   
                    free(rc);
                
           }while(received > 0);
           
        }
        Logger::getInstance(Logger::DEBUG3)<<"ssl com done"<<endl;
        sd = SSL_get_fd(ssl);       // get socket connection 
        SSL_free(ssl);         // release SSL state 
        close(sd);          // close connection 
        Logger::getInstance(Logger::DEBUG3)<<"servlet done"<<endl;
}
/*
void handleNewSSLconnection(SSL *ssl){
    Servlet(ssl);         
}*/