#include "MsgInFactory.h"

#include "IMsgIn.h"
#include "MsgInSignMe.h"
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
#include "MsgInGetNewDevices.h"
#include "MsgInSetGCMID.h"
#include "MsgInDelGCMID.h"
#include "MsgInAlgorithmsRedirect.h"
#include "Msgs/MsgInGamificationRedirect.h"
#include "Msgs/MsgInGetNotifications.h"
#include "Msgs/MsgInNotificationRead.h"
#include "MsgInGetUserID.h"
#include "MsgInGetUserInfo.h"
#include "Msgs/MsgInSignUp.h"
#include "Msgs/MsgInDelAdapter.h"

#include <algorithm> 

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
        Logger::error()<< "XML [" << msg << "] parsed with errors" << endl;
        Logger::error()<< "Error description: " << result.description() << endl;
        Logger::error() << "Error offset: " << result.offset << " (error at [..." << (msg + result.offset) << "]"<<endl;
        return new MsgInUnknown(msg,doc); 
    }
    
    
    //doc->save(std::cout, "\t", pugi::format_indent);
    
    string state = doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    
    Logger::getInstance(Logger::DEBUG3) << "factory: msg= "<< state<< endl;

    if(state == MsgInGetAdapters::state)
        return new MsgInGetAdapters(msg, doc);
    if(state == MsgInGetAllDevices::state)
        return new MsgInGetAllDevices(msg, doc);
    if(state == MsgInGetDevs::state)
        return new MsgInGetDevs(msg, doc);
    if(state == MsgInSignMe::state)
        return new MsgInSignMe(msg, doc);
    if(state == MsgInSignUp::state)
        return new MsgInSignUp(msg, doc);
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
    if(state == MsgInGetNewDevices::state)
        return new MsgInGetNewDevices(msg, doc);
    if(state == MsgInSetGCMID::state)
        return new MsgInSetGCMID(msg, doc);
    if(state == MsgInDelGCMID::state)
        return new MsgInDelGCMID(msg, doc);
    if(state == MsgInGetUserID::state)
        return new MsgInGetUserID(msg, doc);
    if(state == MsgInGetUserInfo::state)
        return new MsgInGetUserInfo(msg, doc);
   
    vector<string> algMsgs;
    algMsgs.push_back("addalg");
    algMsgs.push_back("getalg");
    algMsgs.push_back("getalgs");
    algMsgs.push_back("setalg");
    algMsgs.push_back("delalg");
    algMsgs.push_back("getallalgs");
    algMsgs.push_back("passborder");
    
    if(std::find(algMsgs.begin(), algMsgs.end(), state)!=algMsgs.end())
        return new MsgInAlgorithmsRedirect(msg, doc);
    
    vector<string> gamiMsgs = {"getallachievements","setprogresslvl"};
    
    if(std::find(gamiMsgs.begin(), gamiMsgs.end(), state) != gamiMsgs.end())
        return new MsgInGamificationRedirect(msg, doc);
    
    if(state == MsgInGetNotifications::state)
        return new MsgInGetNotifications(msg, doc);
    
    if(state == MsgInNotificationRead::state)
        return new MsgInNotificationRead(msg, doc);    
    if(state == MsgInDelAdapter::state)
        return new MsgInDelAdapter(msg, doc);
    
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

