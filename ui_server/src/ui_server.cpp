#include "ui_server.h"

#include "../DAO/DAO.h"
#include "../DAO/DAOUsers.h"
#include "../DAO/DAOGateways.h"
#include "../DAO/DAODevices.h"
#include "../DAO/DAOMobileDevices.h"
#include "../DAO/DAOLocations.h"
#include "../DAO/DAOUsersGateways.h"
#include "../DAO/DAONotification.h"
#include "../DAO/DAOPushNotificationService.h"
#include "../DAO/DAOlogs.h"

#include "Config.h"

#include "SecuredAsioServlet.h"
#include "SessionsTable.h"

#include <soci.h>
#include <postgresql/soci-postgresql.h>

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <thread>
#include <string>
#include <iostream>
#include <math.h>
#include <cmath>
#include "sslServlet.h"
#include "RequestHandlerFactory.h"

using namespace std;
using namespace soci;

void startASIONetwork(Config & config){
    try
        {
            std::string clientDelim("</request>");
            std::string serverDelim("</response>");
            int threadCount = config.getServerThreadsNumber();
            int timePeriod = 30;

            asio::io_service io_service;

            SecuredAsioServlet servlet(io_service, config.getServerPort(),
              config.getCertFile(), config.getKeyFile(),
              clientDelim, serverDelim,
              threadCount, timePeriod);
            servlet.startAccept();
            servlet.run();
        }
        catch (std::exception& e)
        {
          std::cerr << "Exception: " << e.what() << "\n";
        }
}

void startOpenSSLNetwork(Config & config){
    
    Logger::debug()<< "setting SSL context..."<< endl;
    SSL_CTX *ctx;
    int server;
    atomic<int>* threadCounter = new atomic<int>(0);

    //initSSL
    SSL_library_init();
    ctx = InitServerCTX();        /* initialize SSL */
    LoadCertificates(ctx, config.getCertFile().c_str(), config.getKeyFile().c_str()); /* load certs */
    server = OpenListener(config.getServerPort());    /* create server socket */

    
    Logger::debug()<< "SSL context is set"<< endl;
    
    DAOContainer dc(config.getDBConnectionString(), config.getDBSessionsNumber());
    SessionsTable st(config.getComTableSleepPeriodMs(), config.getComTableMaxInactivityMs(), config.isDebugMode());
    AdaServerCommunicator ada(config.getAdaServerPort());
    
    RequestHandlerFactory requesthandlerFac(dc, ada, st);
    
    while (1)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);

        //wait for phone connection (unsecured))
        int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
        Logger::getInstance(Logger::DEBUG3)<<"socket accepted"<<endl;

        //create secured connection
        SSL *ssl;
        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client);

        bool singleThread = false;

        if(singleThread)
        {
            Servlet(ssl, requesthandlerFac);
        }
        else
        {
            //wait if max thread n. is reached
            while(*threadCounter > config.getServerThreadsNumber()) 
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

            Logger::getInstance(Logger::DEBUG2)<<"new thread started, threads c.:"<<(*threadCounter +1)<<endl;

            try
            {
            //create thread which serve phone
                thread t( [ssl, threadCounter, &requesthandlerFac](){
                        (*threadCounter)++;
                        Servlet(ssl, requesthandlerFac);
                        (*threadCounter)--;
                    });
                t.detach();
            }
            catch(const std::system_error& e)
            {
                std::cout << "Caught system_error with code " << e.code() 
                          << " meaning " << e.what() << '\n';
            }

        }
    }
    //wait till threads end their job
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    while(*threadCounter)
    {
        Logger::getInstance(Logger::DEBUG2)<<"wait- threads c.:"<<(*threadCounter +1)<<endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    delete threadCounter;
    //close server socket 
    close(server);
    //release ssl
    SSL_CTX_free(ctx);
}

void addSessionWrapper(vector<string> vs)
{
    cout << "adder hello" << vs[1] << endl;
}


int main(int argc, char** argv)
{       
    
    Config config;
    if(argc>=2)
    {
        try
        {
            string configFile = argv[1];
            Logger::debug3() << "Try to load " << configFile <<endl;
            config.loadXml(configFile);
        }
        catch(string & e)
        {
            Logger::fatal() << "Cannot load config file. " << e <<endl;
            return 1;
        }
    }
    else
    {
            Logger::fatal() << "Set config file as first argument" <<endl;
            return 1;
    }
    
    
    int serverPort = config.getServerPort();

    if(config.isLogsPrintedToCout()){
        Logger::getInstance().setOutputToStdout();
    }
    else
    {
        string logsFolder = config.getLogsFolder();
        Logger::getInstance().setOutputToFiles(logsFolder);
    }

    Logger::getInstance().setVerbose( config.getVerbosity() );
        
    Logger::debug()<< "start with port" << serverPort << endl ;
    Logger::debug()<< "threads: " << config.getServerThreadsNumber() << endl;

    startOpenSSLNetwork(config);
    
    Logger::debug() << "ui_server done" << endl << "###############################################" << endl;
    return 0;
}