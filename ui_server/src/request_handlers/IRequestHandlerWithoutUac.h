#ifndef IMSGINLOGINUNWANTED_H
#define	IMSGINLOGINUNWANTED_H

#include "IRequestHandler.h"


class IRequestHandlerWithoutUac 
    :   public IRequestHandler
{
public:
    IRequestHandlerWithoutUac(std::shared_ptr<pugi::xml_document> doc);
    virtual ~IRequestHandlerWithoutUac();    
    virtual bool isComIdValid();
    virtual enumAccessStatus checkAccess();

private:

};

#endif	/* IMSGINLOGINUNWANTED_H */

