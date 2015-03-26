//Defines the entry point for the console application.

#include "ui_server.h"
#include "msgInGetCondition.h"
#include "../DAO/DAO.h"
#include "../DAO/DAOUsers.h"
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
//http://pugixml.googlecode.com/svn-history/r605/trunk/docs/samples/modify_add.cpp
    //load XML file from argv[1]
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

        Logger::getInstance().setVerbose( Config::getInstance().getVerbosity() );
        
        Logger::getInstance(Logger::FATAL) << "start with port"<<serverPort << endl ;
        Logger::getInstance(Logger::FATAL) << "threads: "<<Config::getInstance().getServerThreadsNumber() << endl;
        Logger::getInstance(Logger::FATAL) << "DB: "<<Config::getInstance().getDBConnectionString() << "Fgh" << endl;
        Logger::getInstance(Logger::FATAL) << "logs will be stored in : " << Logger::getInstance().getFileName() <<" and (if not std::out ) file will be changed every day "<< endl;

        
       // ComTable::getInstance().startComTableCleaner(Config::getInstance().getComTableSleepPeriodMs(), Config::getInstance().getComTableMaxInactivityMs() );
        try{                        
            DBConnector::getInstance().setConnectionStringAndOpenSessions( Config::getInstance().getDBConnectionString() , Config::getInstance().getDBSessionsNumber());
        }
        catch (soci::soci_error const & e)
        {
            Logger::getInstance(Logger::FATAL) << "DB error (soci), probably cant set connection, more:" << e.what() << endl;
            return 1;
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
        /*
        resolveMsg( "<com ver=\"2.3\"  state=\"getuid\" email=\"new22@gmail.com\" gid=\"1111\" gt=\"1\" pid=\"11\" loc=\"cs\" />");
        resolveMsg( "<com ver=\"2.3\"  state=\"getuid\" email=\"new22@gmail.com\" gid=\"1111\" gt=\"1\" pid=\"111\" loc=\"cs\" />");
        resolveMsg( "<com ver=\"2.3\"  state=\"getuid\" email=\"new33@gmail.com\" gid=\"11111\" gt=\"1\" pid=\"11111\"  loc=\"cs\" />");
*/
       // resolveMsg( "<com ver=\"2.3\"  state=\"getuid\" email=\"newww@gmail.com\" gid=\"33\" gt=\"1\" pid=\"34\" loc=\"cs\" />");
       
      //resolveMsg("<com ver=\"2.3\"  uid=\"15\" state=\"setgcmid\"  gcmid=\"new\" />");
    //resolveMsg("<com ver=\"2.3\"  uid=\"14\" state=\"delgcmid\"  email=\"11111@v\" gcmid=\"new\" />");
    //resolveMsg("<com ver=\"2.3\"  uid=\"15\" state=\"setgcmid\"  gcmid=\"new\" />");


    
      //DBConnector::getInstance().DEBUGexec("INSERT INTO USERS (user_id,MAIL) VALUES (1,'dummy@gmail.com');" );
      //DBConnector::getInstance().DEBUGexec("insert into mobile_devices(fk_user_id, id, token) values(1,1,1);" );
      
       // resolveMsg( "<com ver=\"2.3\"  uid=\"9\" state=\"addadapter\" aid=\"10\"     aname=\"home\"  />");
        //resolveMsg( "<com ver=\"2.3\"  state=\"getuid\" email=\"n11@gmail.com\" gid=\"99191\" gt=\"1\" pid=\"1100\" loc=\"cs\" />");
    //resolveMsg("<com ver=\"2.3\" state=\"addadapter\" uid=\"40\" aid=\"1234567890123456\" aname=\"test\" />");
    
       // resolveMsg("<com ver=\"2.3\" state=\"delalg\" uid=\"42\" aid=\"1234567890123456\" aname=\"test\" />"); 
        
        
        //resolveMsg( "<com ver=\"2.3\"  state=\"getuid\" email=\"n11@gmail.com\" gid=\"99191\" gt=\"1\" pid=\"1100\" loc=\"cs\" />");
        resolveMsg( "<com ver=\"2.4\"  state=\"signme\" srv=\"google\" bt=\"Rv8FZr2ktR\"  />");
         resolveMsg( "<com ver=\"2.4\"  state=\"addalg\" bt=\"Rv8FZr2ktR\"  />");
        
       /* User u;
        MobileDevice m;
        
        u.mail = "a@a.xx";
        m.token=111;
        m.mobile_id ="abc";
        DAOUsers::getInstance().upsertUserWithMobileDevice(u, m);
        */
      //  return 0;
       SSL_CTX *ctx;
       int server;
       atomic<int>* threadCounter = new atomic<int>(0);
        
       //initSSL
        SSL_library_init();
        ctx = InitServerCTX();        /* initialize SSL */
        LoadCertificates(ctx, CertFile, KeyFile); /* load certs */
        server = OpenListener(serverPort);    /* create server socket */
        
        
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
                
                //create thread which serve phone
                thread *t =  new thread( [ssl, threadCounter, resolveFunc](){
                    (*threadCounter)++;
                    Servlet(ssl,resolveFunc);
                    (*threadCounter)--;
                });
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
        Logger::getInstance(Logger::FATAL) << "ui_server done\n";
        return 0;
}
