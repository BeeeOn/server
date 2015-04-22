#ifndef MSGFACTORY_H
#define	MSGFACTORY_H

#include <string>
#include "IMsgIn.h"

class MsgInFactory
{
private:
        MsgInFactory(void);
public:
        static MsgInFactory& getInstance();
	~MsgInFactory(void);
	IMsgIn* createMsg(char* msg);
};

#endif /* MSGFACTORY_H */