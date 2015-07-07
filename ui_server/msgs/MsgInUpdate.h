#ifndef MSGINUPDATE_H
#define	MSGINUPDATE_H

#include "IMsgIn.h"
#include "MsgOutPartial.h"
#include "DBConnector.h"
#include "MsgOutReSign.h"
#include "MsgOutCommunication.h"
#include "IMsgInLoginAndAdapterAccessRequired.h"

using namespace std;
class MsgInUpdate : public IMsgInLoginAndAdapterAccessRequired
{
public:
	MsgInUpdate(char* msg, pugi::xml_document* doc);
	virtual ~MsgInUpdate(void);
	virtual IMsgOut* createResponseMsgOut();

	static const std::string state;
};

#endif /* MSGINUPDATE_H */