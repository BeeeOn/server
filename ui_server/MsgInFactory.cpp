#include "MsgInFactory.h"

#include "ServerException.h"

#include <algorithm> 

#include "msgs/IMsgIn.h"

#include "msgs/MsgInSignMe.h"
#include "msgs/MsgInGetDevices.h"
#include "msgs/MsgInUnknown.h"
#include "msgs/MsgInGetAdapters.h"
#include "msgs/MsgInGetRooms.h"
#include "msgs/MsgInGetLog.h"
#include "msgs/MsgInAddRoom.h"
#include "msgs/MsgInDelRoom.h"
#include "msgs/MsgInSetTimeZone.h"
#include "msgs/MsgInGetTimeZone.h"
#include "msgs/MsgInGetAccount.h"
#include "msgs/MsgInDelAccount.h"
#include "msgs/MsgInChangeAccount.h"
#include "msgs/MsgInAddAccount.h"
#include "msgs/MsgInAddView.h"
#include "msgs/MsgInUpdateView.h"
#include "msgs/MsgInDelView.h"
#include "msgs/MsgInGetViews.h"
#include "msgs/MsgInGetAllDevices.h"
#include "msgs/MsgInDelDevice.h"
#include "msgs/MsgInAdapterListen.h"
#include "msgs/MsgInSwitch.h"
#include "msgs/MsgInAddAdapter.h"
#include "msgs/MsgInGetNewDevices.h"
#include "msgs/MsgInSetGCMID.h"
#include "msgs/MsgInDelGCMID.h"
#include "msgs/MsgInAlgorithmsRedirect.h"
#include "msgs/MsgInGamificationRedirect.h"
#include "msgs/MsgInGetNotifications.h"
#include "msgs/MsgInNotificationRead.h"
#include "msgs/MsgInGetUserID.h"
#include "msgs/MsgInGetUserInfo.h"
#include "msgs/MsgInSignUp.h"
#include "msgs/MsgInDelAdapter.h"



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

IMsgIn* MsgInFactory::createMsg(const char* msg)
{
    
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load(msg);
        
    if (!result)
    {
        Logger::error()<< "XML [" << msg << "] parsed with errors" << endl;
        Logger::error()<< "Error description: " << result.description() << endl;
        Logger::error() << "Error offset: " << result.offset << " (error at [..." << (msg + result.offset) << "]"<<endl;
        return new MsgInUnknown(doc); 
    }
    
    
    //doc->save(std::cout, "\t", pugi::format_indent);
    
    string state = doc->child(P_COMMUNICATION).attribute(P_STATE).value();
    
    Logger::getInstance(Logger::DEBUG3) << "factory: msg= "<< state<< endl;

    if(state == MsgInGetAdapters::state)
        return new MsgInGetAdapters(doc);
    if(state == MsgInGetAllDevices::state)
        return new MsgInGetAllDevices(doc);
    if(state == MsgInGetDevs::state)
        return new MsgInGetDevs(doc);
    if(state == MsgInSignMe::state)
        return new MsgInSignMe(doc);
    if(state == MsgInSignUp::state)
        return new MsgInSignUp(doc);
    if(state == MsgInGetRooms::state)
        return new MsgInGetRooms(doc);
    if(state == MsgInAddRoom::state)
        return new MsgInAddRoom(doc);
    if(state == MsgInDelRoom::state)
        return new MsgInDelRoom(doc);
    if(state == MsgInSetTimeZone::state)
        return new MsgInSetTimeZone(doc);
    if(state == MsgInGetTimeZone::state)
        return new MsgInGetTimeZone(doc);
    if(state == MsgInGetConAccount::state)
        return new MsgInGetConAccount(doc);
    if(state == MsgInDelConAccount::state)
        return new MsgInDelConAccount(doc);
    if(state == MsgInChangeConAccount::state)
        return new MsgInChangeConAccount(doc);
    if(state == MsgInAddAccount::state)
        return new MsgInAddAccount(doc);
    if(state == MsgInAddView::state)
        return new MsgInAddView(doc);
    if(state == MsgInUpdateView::state)
        return new MsgInUpdateView(doc);
    if(state == MsgInDelView::state)
        return new MsgInDelView(doc);
    if(state == MsgInGetViews::state)
        return new MsgInGetViews(doc);
    if(state == MsgInDelDevice::state)
        return new MsgInDelDevice(doc);
    if(state == MsgInAdapterListen::state)
        return new MsgInAdapterListen(doc);
    if(state == MsgInSwitch::state)
        return new MsgInSwitch(doc);
    if(state == MsgInAddAdapter::state)
        return new MsgInAddAdapter(doc);
    if(state == MsgInGetNewDevices::state)
        return new MsgInGetNewDevices(doc);
    if(state == MsgInSetGCMID::state)
        return new MsgInSetGCMID(doc);
    if(state == MsgInDelGCMID::state)
        return new MsgInDelGCMID(doc);
    if(state == MsgInGetUserID::state)
        return new MsgInGetUserID(doc);
    if(state == MsgInGetUserInfo::state)
        return new MsgInGetUserInfo(doc);
   
    vector<string> algMsgs;
    algMsgs.push_back("addalg");
    algMsgs.push_back("getalg");
    algMsgs.push_back("getalgs");
    algMsgs.push_back("setalg");
    algMsgs.push_back("delalg");
    algMsgs.push_back("getallalgs");
    algMsgs.push_back("passborder");
    
    if(std::find(algMsgs.begin(), algMsgs.end(), state)!=algMsgs.end())
        return new MsgInAlgorithmsRedirect(doc);
    
    vector<string> gamiMsgs = {"getallachievements","setprogresslvl"};
    
    if(std::find(gamiMsgs.begin(), gamiMsgs.end(), state) != gamiMsgs.end())
        return new MsgInGamificationRedirect(doc);
    
    if(state == MsgInGetNotifications::state)
        return new MsgInGetNotifications(doc);
    
    if(state == MsgInNotificationRead::state)
        return new MsgInNotificationRead(doc);    
    if(state == MsgInDelAdapter::state)
        return new MsgInDelAdapter(doc);
    

    Logger::getInstance(Logger::ERROR)<<"UNKNOWN MSG"<<endl;
    return new MsgInUnknown(doc);
}
/*
 switch ( state ) {
 case MsgInGetAdapters::state :
 return new MsgInGetAdapters(doc);
 case MsgInInit::state :
 return new MsgInInit(doc);
 case MsgInUpdate::state :
 return new MsgInUpdate(doc);
 case MsgInSignUp::state :
 return new MsgInSignUp(doc);
 case MsgInPartial::state :
 return new MsgInPartial(doc);
 case MsgInUpdateRooms::state :
 return new MsgInUpdateRooms(doc);
 case MsgInGetRooms::state :
 return new MsgInGetRooms(doc);
 case MsgInLogName::state :
 return new MsgInLogName(doc);
 case MsgInAddRoom::state :
 return new MsgInAddRoom(doc);
 case MsgInDelRoom::state :
 return new MsgInDelRoom(doc);
 case MsgInSetTimeZone::state :
 return new MsgInSetTimeZone(doc);
 case MsgInGetTimeZone::state :
 return new MsgInGetTimeZone(doc);
 case MsgInGetConAccount::state :
 return new MsgInGetConAccount(doc);
 case MsgInDelConAccount::state :
 return new MsgInDelConAccount(doc);
 case MsgInChangeConAccount::state :
 return new MsgInChangeConAccount(doc);
 case MsgInAddConAccount::state :
 return new MsgInAddConAccount(doc);
 case MsgInAddView::state :
 return new MsgInAddView(doc);
 case MsgInUpdateView::state :
 return new MsgInUpdateView(doc);
 case MsgInDelView::state :
 return new MsgInDelView(doc);
 case MsgInGetViews::state :
 return new MsgInGetViews(doc);
 default:
 Logger::getInstance(Logger::DEBUG)<<"UNKNOWN MSG"<<endl;
 return new MsgInUnknown(msg,doc);
 }
 */

