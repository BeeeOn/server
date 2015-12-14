#include "IRequestHandlerWithoutUac.h"

IRequestHandlerWithoutUac::IRequestHandlerWithoutUac(std::shared_ptr<pugi::xml_document> doc)
    :   IRequestHandler(doc) 
{
}

IRequestHandlerWithoutUac::~IRequestHandlerWithoutUac() 
{
}

bool IRequestHandlerWithoutUac::isComIdValid()
{
    return true;
}

enumAccessStatus IRequestHandlerWithoutUac::checkAccess()
{
    return GRANTED;
}
