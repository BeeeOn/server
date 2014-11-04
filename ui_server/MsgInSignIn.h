#ifndef MSGINSIGNIN_H
#define	MSGINSIGNIN_H

#include "IMsgIn.h"
#include "pugixml.hpp"
#include <string>
#include <iostream>
#include "DBConnector.h"
#include "ComTable.h"
#include "gTokenChecker.h"
#include "IMsgInLoginUnwanted.h"

using namespace std;
class MsgInSignIn : public IMsgInLoginUnwanted
{
public:
    MsgInSignIn(char* msg, pugi::xml_document* doc);
    ~MsgInSignIn(void);
    virtual string createResponseMsgOut();
    virtual int getMsgAuthorization();
    
    
    static const std::string state;
};

#endif /* MSGINSIGNIN_H */
