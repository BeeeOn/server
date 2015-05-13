//Defines the entry point for the console application.

#include "ui_server.h"
#include "msgInGetCondition.h"
#include "../DAO/DAO.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOAdapters.h"
#include "../DAO/DAODevices.h"
#include "../DAO/DAOMobileDevices.h"
#include "../DAO/DAORooms.h"
#include "../DAO/DAOUsersAdapters.h"
#include "../DAO/DAONotification.h"

#include "SocketServer.h"
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
        Logger::getInstance(Logger::FATAL) << "SIGINT catched"<<endl;
        serverStop = true;
       // exit(0);//ODSTRANENI toho vede k vypisum ve valgrindu, tak si to zkontroluj!!!
        SocketClient sc(serverPort);
        sc.write("end");
    }
}

void serverF(int port){
    SocketServer ss;
    ss.start(port);
}
int main(int argc, char** argv)
{   
//http://pugixml.googlecode.com/svn-history/r605/trunk/docs/samples/modify_add.cpp
    //load XML file from argv[1]

    if(argc>=2){
        try{
            Config::getInstance().loadXml(argv[1]);
        }catch(string & e){
            Logger::fatal() << "Cannot load config file. " <<e<<endl;
            return 1;
        }
    }
    
    serverPort = Config::getInstance().getServerPort();

    if (signal(SIGINT, sig_handler) == SIG_ERR)
    {
            Logger::error()<< "unable to catch signal SIGINT "<<endl;
    }

    if(Config::getInstance().isLogsPrintedToCout()){
        Logger::getInstance().setOutputToStdout();
    }else{
        string logsFolder = Config::getInstance().getLogsFolder();
        Logger::getInstance().setOutputToFiles(logsFolder);
    }

    Logger::getInstance().setVerbose( Config::getInstance().getVerbosity() );
        
        



    Logger::debug()<< "start with port"<<serverPort << endl ;
    Logger::debug()<< "threads: "<<Config::getInstance().getServerThreadsNumber() << endl;
    Logger::debug() << "DB: "<<Config::getInstance().getDBConnectionString() << endl;
    if(Logger::getInstance().isOutputSetToCout()){
        Logger::debug()<< "logs will be printed on cout"<< endl;
    }else{
        cout << "logs will be stored in : " << Logger::getInstance().getFileName() <<" and file will be changed every day "<< endl;
        Logger::debug()<< "logs will be stored in : " << Logger::getInstance().getFileName() <<" and file will be changed every day "<< endl;
    }

   // ComTable::getInstance().startComTableCleaner(Config::getInstance().getComTableSleepPeriodMs(), Config::getInstance().getComTableMaxInactivityMs() );
    try{      
        Logger::debug()<< "setting connections to DB..."<< endl;
        
        string conString = Config::getInstance().getDBConnectionString();
        
        DBConnector::getInstance().setConnectionStringAndOpenSessions( conString , Config::getInstance().getDBSessionsNumber());
        DAOUsers::getInstance().setConnectionStringAndOpenSessions(conString, 2);
        DAOAdapters::getInstance().setConnectionStringAndOpenSessions(conString, 2);
        DAODevices::getInstance().setConnectionStringAndOpenSessions(conString, 2);
        DAORooms::getInstance().setConnectionStringAndOpenSessions(conString, 2);
        DAOMobileDevices::getInstance().setConnectionStringAndOpenSessions(conString, 3);
        DAOUsersAdapters::getInstance().setConnectionStringAndOpenSessions(conString, 2);
        DAONotification::getInstance().setConnectionStringAndOpenSessions(conString, 2);
        
        Logger::debug()<< "connection to DB is set"<< endl;
    }
    catch (soci::soci_error const & e)
    {
        Logger::fatal()<< "DB error (soci), probably cant set connection, more:" << e.what()<< endl;
        return 1;
    }
        

        resolveMsg( "<com ver=\"2.4\"  state=\"getadapters\" email=\"new22@gmail.com\" gid=\"1111\" gt=\"1\" pid=\"11\" loc=\"cs\" />");
        resolveMsg( "<com ver=\"2.5\"  state=\"getadapters\" email=\"new22@gmail.com\" gid=\"1111\" gt=\"1\" pid=\"111\" loc=\"cs\" />");
        resolveMsg( "<com ver=\"1.3\"  state=\"getadapters\" email=\"new33@gmail.com\" gid=\"11111\" gt=\"1\" pid=\"11111\"  loc=\"cs\" />");
    resolveMsg( "<com ver=\"2.4\" state=\"signup\" srv=\"facebook\"><par fbt=\"CAAMVd7mjduYBAKsnl5i2iJljZAG1A6PDraitTxF2v91iDDoOwZA5uOSxYCpo2a0WZC7ZB8I8n3hXEFrgBBZCEoO6HZAtENfNO72n8DmZAYdVYknltIY50g1ACzkhPavWnCtOkGBdD68VnwnfhLtZA00SjWw9QiZCzjg09ZBVKPSZBPqKZAGFDawZAWZBV82KWiCp4uMruh5AiBcs5ihHTsENM0d5CfGx0bfEwo0F7IIGHOUGv0IJYSZBOZCmZANPc\" /></com>");

        Logger::debug()<< "setting SSL context..."<< endl;
       SSL_CTX *ctx;
       int server;
       atomic<int>* threadCounter = new atomic<int>(0);
        
       //initSSL
        SSL_library_init();
        ctx = InitServerCTX();        /* initialize SSL */
        LoadCertificates(ctx, CertFile, KeyFile); /* load certs */
        server = OpenListener(serverPort);    /* create server socket */
        
        Logger::debug()<< "SSL context is set"<< endl;
        while (!serverStop) {
            struct sockaddr_in addr;
            socklen_t len = sizeof(addr);
            
            //wait for phone connection (unsecured))
            int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
            Logger::getInstance(Logger::DEBUG3)<<"socket accepted"<<endl;
            
            
            //create secured connection
            SSL *ssl;
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, client);
            
            std::function<string(char*)> resolveFunc;// = &resolveMsg;
            
            if(serverStop)
                break;
            
            
            bool singleThread = false;
            
            if(singleThread){
                Servlet(ssl, resolveFunc);
            }else{
                //wait if max thread n. is reached
                while(*threadCounter > Config::getInstance().getServerThreadsNumber()) 
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));

                Logger::getInstance(Logger::DEBUG2)<<"new thread started, threads c.:"<<(*threadCounter +1)<<endl;
                
                
                try{
                //create thread which serve phone
                    new thread( [ssl, threadCounter, resolveFunc](){
                        (*threadCounter)++;
                        Servlet(ssl,resolveFunc);
                        (*threadCounter)--;
                    });
                }catch(const std::system_error& e) {
                    std::cout << "Caught system_error with code " << e.code() 
                              << " meaning " << e.what() << '\n';
                }
            
            }
        }
        //wait till threads end their job
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        while(*threadCounter){
            Logger::getInstance(Logger::DEBUG2)<<"wait- threads c.:"<<(*threadCounter +1)<<endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        
        //close server socket 
        close(server);
        //release ssl
        SSL_CTX_free(ctx);
        Logger::debug() << "ui_server done" << endl << "###############################################" << endl;
        return 0;
}
