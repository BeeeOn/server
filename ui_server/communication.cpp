#include "communication.h"
#include "MsgInFactory.h"
#include "../ui_logger/Logger.h"
#include "ServerException.h"
#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>
using namespace std;

string resolveMsg(char* msg)
{       
    string response;
    MsgInFactory msgFactory = MsgInFactory::getInstance();
    IMsgIn* msgIn=NULL;
    
    Logger::getInstance(Logger::DEBUG)<< "MSGIN: "<<msg<< endl;
    
    msgIn = msgFactory.createMsg(msg);
    
    try
    {     
        if(msgIn->checkProtocolVersion() != true)//user do not have permission to use this MSG
            throw ServerException(ServerException::VERSION);
        
        switch( msgIn->checkAccess() ){
            case GRANTED:
                Logger::getInstance(Logger::DEBUG3)<< "access granted "<<endl;
                response =  msgIn->createResponseMsgOut();
                break;
            case FORBIDDEN_NOT_LOGGED:
                Logger::getInstance(Logger::DEBUG3)<< "user not logged: "<<endl;
                throw ServerException(ServerException::RESIGN);
                break;
            case FORBIDDEN_WRONG_RIGHTS:
                Logger::getInstance(Logger::DEBUG3)<< "user dont have rights"<<endl;
                throw ServerException(ServerException::MSG_RIGHT);
                break;
            default:
                throw ServerException(ServerException::UNKNOWN_ERROR);
                break;
        }
        
        
    }
    catch (ServerException & e)
    {
        Logger::getInstance(Logger::ERROR) << "Server error: " << e.getErrCode()<< endl;
        response = msgIn->envelopeResponse(R_FALSE, e.getErrText(), e.getErrCode());
    }
    catch (soci::postgresql_soci_error const & e)
    {
        Logger::getInstance(Logger::ERROR) << "DB error (psql_soci): " << e.what() <<"| sql state:"<<e.sqlstate()<< endl;
        
        response = msgIn->envelopeResponse(R_FALSE);
    }
    catch (soci::soci_error const & e)
    {
        //TODO  mozna nedelat false 
        Logger::getInstance(Logger::ERROR) << "DB error (soci): " << e.what() << endl;
        
        response = msgIn->envelopeResponse(R_FALSE);
    }catch (exception& e)
  {
            Logger::getInstance(Logger::FATAL) <<"ERROR ! :"<<e.what()<<endl;
        
        response = msgIn->envelopeResponse(R_FALSE);
  }
    catch (...)
    {
        //TODO  mozna nedelat false 
        Logger::getInstance(Logger::FATAL) <<"ERROR !!!!!!!!!!!!!!!!!!"<<endl;
        
        response = msgIn->envelopeResponse(R_FALSE);
    }
    
    Logger::getInstance(Logger::DEBUG)<< "MSGOUT: " << response<<"\n";
    
    delete msgIn;
    
    return response;
}

string resolveMsg(string msg){
    string response = resolveMsg(msg.c_str());
    return response;
}