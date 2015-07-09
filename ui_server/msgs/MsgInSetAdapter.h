/* 
 * File:   MsgInSetAdapter.h
 * Author: Pavel
 *
 * Created on 9. července 2015, 13:21
 */

#ifndef MSGINSETADAPTER_H
#define	MSGINSETADAPTER_H

#include "IMsgInLoginRequired.h"
#include "../../DAO/DAOAdapters.h"


class MsgInSetAdapter : public IMsgInLoginRequired{
public:
    MsgInSetAdapter(pugi::xml_document* doc);

    virtual ~MsgInSetAdapter();

    virtual int getMsgAuthorization();
    
    virtual std::string createResponseMsgOut();
        
    static const std::string state;
private:

};

#endif	/* MSGINSETADAPTER_H */
