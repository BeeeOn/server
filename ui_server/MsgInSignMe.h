#ifndef MSGINSIGNME_H
#define	MSGINSIGNME_H

#include "IMsgIn.h"
#include "DBConnector.h"

#include "ComTable.h"
#include "gTokenChecker.h"
#include "fTokenChecker.h"
#include "IMsgInLoginUnwanted.h"

class MsgInSignMe :
	public IMsgInFreeAccess
{
            public:
                MsgInSignMe(char* msg, pugi::xml_document* doc);
                virtual ~MsgInSignMe(void);
                virtual string createResponseMsgOut();
                
                virtual int getMsgAuthorization();
                
                
                static const std::string state;
                private :
                    string getnewIHAtoken();
        };
        
#endif /* MSGINSIGNME_H */