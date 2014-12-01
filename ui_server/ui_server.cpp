// Server.cpp : Defines the entry point for the console application.

#include "ui_server.h"
#include "msgInGetCondition.h"
// uncoment if you want print debug reports
//#define DEBUG 1

#define FAIL    -1

#define SERVER_PORT 4565


/**
 * certificate paths
 */
char CertFile[] = "./cert/ant-2.fit.vutbr.cz.crt";
char KeyFile[]  = "./cert/ant-2.fit.vutbr.cz.key";


using namespace std;




bool serverStop;
int serverPort;


void sig_handler(int signo)
{
    if (signo == SIGINT)
    {
        serverStop = true;
        exit(0);//ODSTRANENI toho vede k vypisum ve valgrindu, tak si to zkontroluj!!!
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
        Logger::getInstance(Logger::FATAL) << "threads: "<<Config::getInstance().getServerThreadsNumber() << endl;
       // ComTable::getInstance().startComTableCleaner(Config::getInstance().getComTableSleepPeriodMs(), Config::getInstance().getComTableMaxInactivityMs() );
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

        /*
         resolveMsg( "<com ver=\"2.3\"  state=\"getuid\" email=\"new22@gmail.com\" gid=\"1111\" gt=\"1\"  loc=\"cs\" />");
        resolveMsg( "<com uid=\"1\" state=\"getdevs\" ver=\"2.3\"><adapter id=\"10\"><dev id=\"0.0.10.1\"><part type=\"1\" /><part type=\"2\" /></dev></adapter></com>");
        resolveMsg( "<com uid=\"1\" state=\"getnewdevs\" ver=\"2.3\" aid=\"10\"></com>");
        return 0;
        resolveMsg( "<com ver=\"2.3\"  state=\"getuid\" email=\"new22@gmail.com\" gid=\"1111\" gt=\"1\"  loc=\"cs\" />");
        resolveMsg( "<com ver=\"2.3\" uid=\"2\" state=\"getalldevs\" aid=\"20\" />");
        resolveMsg( "<com ver=\"2.3\" uid=\"2\" state=\"getdevs\" ><adapter id=\"20\"><dev id=\"0.0.20.1\"><part type=\"1\" /></dev><dev id=\"0.0.10.2\"><part type=\"1\" /></dev></adapter></com>");
        resolveMsg( "<com ver=\"2.3\" uid=\"2\" state=\"getlog\" from=\"1377684610\" ftype=\"avg\" interval=\"0\" aid=\"21\" did=\"0.0.21.1\" dtype=\"1\" />" );
       resolveMsg( "<com ver=\"2.3\" uid=\"1\" state=\"getnewdevs\" aid=\"10\"  />" );
*/
        
       
       
       
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
            
            std::function<string(char*)> resolveFunc;// = &resolveMsg;
            
            if(serverStop)break;
            
            if(0){
              //  Servlet(ssl, resolveFunc);
            }else{
                while(*threadCounter > Config::getInstance().getServerThreadsNumber()) 
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));

                Logger::getInstance(Logger::DEBUG2)<<"threads c.:"<<(*threadCounter +1)<<endl;
                Logger::getInstance(Logger::DEBUG2)<<"new thread started"<<endl;
                thread *t =  new thread( [ssl, threadCounter,resolveFunc](){
                    (*threadCounter)++;
                    Servlet(ssl,resolveFunc);
                    (*threadCounter)--;
                });
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        while(*threadCounter){
            Logger::getInstance(Logger::DEBUG2)<<"wait- threads c.:"<<(*threadCounter +1)<<endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        close(server);          /* close server socket */
        SSL_CTX_free(ctx);         /* release context */
        Logger::getInstance(Logger::FATAL) << "Server main thread exit\n";
        return 0;
}