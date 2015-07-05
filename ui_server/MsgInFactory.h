#ifndef MSGFACTORY_H
#define	MSGFACTORY_H

#include <string>
#include "msgs/IMsgIn.h"

class MsgInFactory
{
private:
        MsgInFactory(void);
public:
        static MsgInFactory& getInstance();
	~MsgInFactory(void);
	IMsgIn* createMsg(const char* msg);
};

#endif /* MSGFACTORY_H */