#ifndef MSGETADAPTERS_H
#define	MSGETADAPTERS_H

#include "IRequestHandler.h"
#include "../DAO/DAOUsers.h"
#include "IRequestHandlerWithLoginRequired.h"

class GatesGetAllRequestHandler
    :   public IRequestHandlerWithLoginRequired
{
    public:
        GatesGetAllRequestHandler(std::shared_ptr<pugi::xml_document> doc, DAOUsers & daoUsers, SessionsTable & sessionTable);
        virtual ~GatesGetAllRequestHandler(void);
        
        virtual std::string createResponseMsgOut();
        
        static const std::string state;
    private:
        DAOUsers & _daoUsers;
};
#endif /* MSGETADAPTERS_H */
