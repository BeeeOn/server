#ifndef IMSGINLOGINREQUIRED_H
#define	IMSGINLOGINREQUIRED_H

#include "IRequestHandlerWithoutUac.h"
#include "SessionsTable.h"

class IRequestHandlerWithLoginRequired : public IRequestHandlerWithoutUac{
public:
    IRequestHandlerWithLoginRequired(std::shared_ptr<pugi::xml_document> doc, SessionsTable & sessionTable);
    virtual ~IRequestHandlerWithLoginRequired();
    virtual bool isComIdValid();
    virtual enumAccessStatus checkAccess();

    int _userId;
    SessionsTable & _sessionTable;
};

#endif	/* IMSGINLOGINREQUIRED_H */

