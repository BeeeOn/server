#ifndef MSGINSIGNUP_H
#define	MSGINSIGNUP_H

#include "IMsgIn.h"
#include "DBConnector.h"

#include "ComTable.h"
#include "gTokenChecker.h"
#include "IMsgInLoginUnwanted.h"

class MsgInSignUp :
	public IMsgInLoginUnwanted
{
            public:
                MsgInSignUp(char* msg, pugi::xml_document* doc);
                virtual ~MsgInSignUp(void);
                virtual string createResponseMsgOut();
                
                virtual int getMsgAuthorization();
                
                
                static const std::string state;
        };
        
#endif /* MSGINSIGNUP_H */