#ifndef COM_H
#define	COM_H

#include "IMsgIn.h"
class Communicator
{
public:
	Communicator(void);
	virtual ~Communicator(void);
	void resolveMsg(char* msg);
	IMsgOut* createResponse(IMsgIn* msgIn);
	void sendMsgOut(IMsgOut* msgOut);
private:
	int _id;
};

#endif /* COM_H */