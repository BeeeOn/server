#ifndef MSGINUNKNOWN_H
#define	MSGINUNKNOWN_H
#include "IRequestHandlerWithoutUac.h"
class UnknownRequestHandler :
	public IRequestHandlerWithoutUac {
public:
    UnknownRequestHandler(std::shared_ptr<pugi::xml_document> doc);
    virtual ~UnknownRequestHandler();

    virtual int getMsgAuthorization();
    virtual std::string createResponseMsgOut();

	static const std::string state;
private:

};

#endif	/* MSGINUNKNOWN_H */

