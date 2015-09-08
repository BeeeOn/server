#ifndef MSGINSIGNME_H
#define	MSGINSIGNME_H

#include "IMsgIn.h"
#include "DBConnector.h"

#include "gTokenChecker.h"
#include "fTokenChecker.h"
#include "IMsgInLoginUnwanted.h"

class UserLogIn :
	public IMsgInFreeAccess
{
            public:
                UserLogIn(pugi::xml_document* doc);
                virtual ~UserLogIn(void);
                virtual string createResponseMsgOut();
                
                virtual int getMsgAuthorization();
                
                
                static const std::string state;
                private :
                 //   string getnewIHAtoken();
        };
        
#endif /* MSGINSIGNME_H */