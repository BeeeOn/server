#ifndef MSGINSIGNUP_H
#define	MSGINSIGNUP_H

#include "IMsgIn.h"
#include "DBConnector.h"

#include "gTokenChecker.h"
#include "../fTokenChecker.h"
#include "IMsgInLoginUnwanted.h"
#include "IMsgInLoginRequired.h"

class UserRegister :
	public IMsgInFreeAccess
{
            public:
                UserRegister(pugi::xml_document* doc);
                virtual ~UserRegister(void);
                virtual string createResponseMsgOut();
                
                virtual int getMsgAuthorization();
                
                
                static const std::string state;
        };
        
#endif /* MSGINSIGNUP_H */