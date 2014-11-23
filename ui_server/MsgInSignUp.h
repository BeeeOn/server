#ifndef MSGINSIGNUP_H
#define	MSGINSIGNUP_H

#include "IMsgIn.h"
#include "DBConnector.h"

#include "ComTable.h"
#include "gTokenChecker.h"
#include "IMsgInLoginUnwanted.h"

class MsgInGetUID :
	public IMsgInLoginUnwanted
{
            public:
                MsgInGetUID(char* msg, pugi::xml_document* doc);
                virtual ~MsgInGetUID(void);
                virtual string createResponseMsgOut();
                
                virtual int getMsgAuthorization();
                
                
                static const std::string state;
                private :
                    long long int getnewIHAtoken();
                    static long long int _IHAtokenGenerator;
        };
        
#endif /* MSGINSIGNUP_H */