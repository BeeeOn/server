#include "MsgInFactory.h"

#include "IMsgIn.h"
#include "MsgInSignIn.h"
#include "MsgInSignUp.h"
#include "MsgInGetDevices.h"
#include "ComTable.h"
#include "MsgInUnknown.h"
#include "MsgInGetAdapters.h"
#include "MsgInPartial.h"
#include "MsgInUpdateRooms.h"
#include "MsgInGetRooms.h"
#include "MsgInGetLog.h"
#include "MsgInAddRoom.h"
#include "MsgInDelRoom.h"
#include "MsgInSetTimeZone.h"
#include "MsgInGetTimeZone.h"
#include "MsgInGetAccount.h"
#include "MsgInDelAccount.h"
#include "MsgInChangeAccount.h"
#include "MsgInAddAccount.h"
#include "MsgInAddView.h"
#include "MsgInUpdateView.h"
#include "MsgInDelView.h"
#include "MsgInGetViews.h"
#include "MsgInGetAllDevices.h"
#include "ServerException.h"
#include "MsgInDelDevice.h"
#include "MsgInAdapterListen.h"
#include "MsgInSwitch.h"
#include "MsgInAddAdapter.h"
#include "MsgInSetCondition.h"
#include "msgInGetCondition.h"
#include "MsgInGetConditions.h"
#include "MsgInUpdateCondition.h"
#include "MsgInDelCondition.h"
#include "MsgInSetAction.h"
#include "MsgInGetActions.h"
#include "MsgInGetAction.h"
#include "MsgInUpdateAction.h"
#include "MsgInDelAction.h"
#include "MsgInConditionPlusAction.h"
#include "MsgInGetNewDevices.h"

MsgInFactory::MsgInFactory(void)
{
}


MsgInFactory::~MsgInFactory(void)
{
}

MsgInFactory& MsgInFactory::getInstance(){
    static MsgInFactory instance;
    return instance;
}

IMsgIn* MsgInFactory::createMsg(char* msg)
{
    
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load(msg);
        
    if (!result)
    {
        Logger::getInstance(Logger::DEBUG2) << "XML [" << msg << "] parsed with errors\n";
        Logger::getInstance(Logger::DEBUG2) << "Error description: " << result.description() << "\n";
        Logger::getInstance(Logger::DEBUG2) << "Error offset: " << result.offset << " (error at [..." << (msg + result.offset) << "]"<<endl;
        return new MsgInUnknown(msg,doc); 
    }
    
    
    //doc->save(std::cout, "\t", pugi::format_indent);
    
    string state = doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    
    Logger::getInstance(Logger::DEBUG) << "msg: "<< state<< "\n";
    
    if(state == MsgInSignIn::state)
        return new MsgInSignIn(msg, doc);
    if(state == MsgInGetAdapters::state)
        return new MsgInGetAdapters(msg, doc);
    if(state == MsgInGetAllDevices::state)
        return new MsgInGetAllDevices(msg, doc);
    if(state == MsgInGetDevs::state)
        return new MsgInGetDevs(msg, doc);
    if(state == MsgInGetUID::state)
        return new MsgInGetUID(msg, doc);
    if(state == MsgInDevices::state)
        return new MsgInDevices(msg, doc);
    if(state == MsgInUpdateRooms::state)
        return new MsgInUpdateRooms(msg, doc);
    if(state == MsgInGetRooms::state)
        return new MsgInGetRooms(msg, doc);
    if(state == MsgInLogName::state)
        return new MsgInLogName(msg, doc);
    if(state == MsgInAddRoom::state)
        return new MsgInAddRoom(msg, doc);
    if(state == MsgInDelRoom::state)
        return new MsgInDelRoom(msg, doc);
    if(state == MsgInSetTimeZone::state)
        return new MsgInSetTimeZone(msg, doc);
    if(state == MsgInGetTimeZone::state)
        return new MsgInGetTimeZone(msg, doc);
    if(state == MsgInGetConAccount::state)
        return new MsgInGetConAccount(msg, doc);
    if(state == MsgInDelConAccount::state)
        return new MsgInDelConAccount(msg, doc);
    if(state == MsgInChangeConAccount::state)
        return new MsgInChangeConAccount(msg, doc);
    if(state == MsgInAddAccount::state)
        return new MsgInAddAccount(msg, doc);
    if(state == MsgInAddView::state)
        return new MsgInAddView(msg, doc);
    if(state == MsgInUpdateView::state)
        return new MsgInUpdateView(msg, doc);
    if(state == MsgInDelView::state)
        return new MsgInDelView(msg, doc);
    if(state == MsgInGetViews::state)
        return new MsgInGetViews(msg, doc);
    if(state == MsgInDelDevice::state)
        return new MsgInDelDevice(msg, doc);
    if(state == MsgInAdapterListen::state)
        return new MsgInAdapterListen(msg, doc);
    if(state == MsgInSwitch::state)
        return new MsgInSwitch(msg, doc);
    if(state == MsgInAddAdapter::state)
        return new MsgInAddAdapter(msg, doc);
    if(state == MsgInSetCondition::state)
        return new MsgInSetCondition(msg, doc);
    if(state == msgInGetCondition::state)
        return new msgInGetCondition(msg, doc);
    if(state == MsgInGetConditions::state)
        return new MsgInGetConditions(msg, doc);
    if(state == MsgInUpdateCondition::state)
        return new MsgInUpdateCondition(msg, doc);
    if(state == MsgInDelCondition::state)
        return new MsgInDelCondition(msg, doc);
    if(state == MsgInSetAction::state)
        return new MsgInSetAction(msg, doc);
    if(state == MsgInGetActions::state)
        return new MsgInGetActions(msg, doc);
    if(state == MsgInGetAction::state)
        return new MsgInGetAction(msg, doc);
    if(state == MsgInUpdateAction::state)
        return new MsgInUpdateAction(msg, doc);
    if(state == MsgInDelAction::state)
        return new MsgInDelAction(msg, doc);
    if(state == MsgInConditionPlusAction::state)
        return new MsgInConditionPlusAction(msg, doc);
    if(state == MsgInGetNewDevices::state)
        return new MsgInGetNewDevices(msg, doc);
    else{
        Logger::getInstance(Logger::ERROR)<<"UNKNOWN MSG"<<endl;
        return new MsgInUnknown(msg,doc);
    }
}
/*
 switch ( state ) {
 case MsgInGetAdapters::state :
 return new MsgInGetAdapters(msg, doc);
 case MsgInInit::state :
 return new MsgInInit(msg, doc);
 case MsgInUpdate::state :
 return new MsgInUpdate(msg, doc);
 case MsgInSignUp::state :
 return new MsgInSignUp(msg, doc);
 case MsgInPartial::state :
 return new MsgInPartial(msg, doc);
 case MsgInUpdateRooms::state :
 return new MsgInUpdateRooms(msg, doc);
 case MsgInGetRooms::state :
 return new MsgInGetRooms(msg, doc);
 case MsgInLogName::state :
 return new MsgInLogName(msg, doc);
 case MsgInAddRoom::state :
 return new MsgInAddRoom(msg, doc);
 case MsgInDelRoom::state :
 return new MsgInDelRoom(msg, doc);
 case MsgInSetTimeZone::state :
 return new MsgInSetTimeZone(msg, doc);
 case MsgInGetTimeZone::state :
 return new MsgInGetTimeZone(msg, doc);
 case MsgInGetConAccount::state :
 return new MsgInGetConAccount(msg, doc);
 case MsgInDelConAccount::state :
 return new MsgInDelConAccount(msg, doc);
 case MsgInChangeConAccount::state :
 return new MsgInChangeConAccount(msg, doc);
 case MsgInAddConAccount::state :
 return new MsgInAddConAccount(msg, doc);
 case MsgInAddView::state :
 return new MsgInAddView(msg, doc);
 case MsgInUpdateView::state :
 return new MsgInUpdateView(msg, doc);
 case MsgInDelView::state :
 return new MsgInDelView(msg, doc);
 case MsgInGetViews::state :
 return new MsgInGetViews(msg, doc);
 default:
 Logger::getInstance(Logger::DEBUG)<<"UNKNOWN MSG"<<endl;
 return new MsgInUnknown(msg,doc);
 }
 */

