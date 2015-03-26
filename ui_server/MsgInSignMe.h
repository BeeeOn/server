#ifndef MSGINSIGNUP_H
#define	MSGINSIGNUP_H

#include "IMsgIn.h"
#include "DBConnector.h"

#include "ComTable.h"
#include "gTokenChecker.h"
#include "IMsgInLoginUnwanted.h"

class MsgInSignMe :
	public IMsgInLoginUnwanted
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
        
#endif /* MSGINSIGNUP_H */