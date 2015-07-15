#include "MsgInFactory.h"

#include "ServerException.h"

#include <algorithm> 

#include "msgs/IMsgIn.h"

#include "msgs/UserLogIn.h"
#include "msgs/MsgInGetDevices.h"
#include "msgs/MsgInUnknown.h"
#include "msgs/GatesGetConnected.h"
#include "msgs/LocationsGet.h"
#include "msgs/MsgInGetLog.h"
#include "msgs/LocationAdd.h"
#include "msgs/LocationDelete.h"
#include "msgs/MsgInSetTimeZone.h"
#include "msgs/MsgInGetTimeZone.h"
#include "msgs/AccountDel.h"
#include "msgs/AccountUpdate.h"
#include "msgs/AccountAdd.h"
#include "msgs/MsgInAddView.h"
#include "msgs/MsgInUpdateView.h"
#include "msgs/MsgInDelView.h"
#include "msgs/MsgInGetViews.h"
#include "msgs/MsgInGetAllDevices.h"
#include "msgs/MsgInDelDevice.h"
#include "msgs/MsgInAdapterListen.h"
#include "msgs/MsgInSwitch.h"
#include "msgs/GateAdd.h"
#include "msgs/MsgInGetNewDevices.h"
#include "msgs/NotificationSetGCMID.h"
#include "msgs/NotificationEraseGCMID.h"
#include "msgs/MsgInAlgorithmsRedirect.h"
#include "msgs/MsgInGamificationRedirect.h"
#include "msgs/NotificationsGet.h"
#include "msgs/NotificationReaded.h"
#include "msgs/UserGetID.h"
#include "msgs/UserGetInfo.h"
#include "msgs/UserRegister.h"
#include "msgs/GateDelete.h"
#include "msgs/UserLogout.h"
#include "msgs/GateGetInfo.h"
#include "msgs/MsgInSetDevices.h"
#include "msgs/GateUpdate.h"
#include "msgs/AccountGet.h"


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

    if(state == GatesGetConnected::state)
        return new GatesGetConnected(doc);
    if(state == MsgInGetAllDevices::state)
        return new MsgInGetAllDevices(doc);
    if(state == MsgInGetDevs::state)
        return new MsgInGetDevs(doc);
    if(state == UserLogIn::state)
        return new UserLogIn(doc);
    if(state == UserRegister::state)
        return new UserRegister(doc);
    if(state == LocationsGet::state)
        return new LocationsGet(doc);
    if(state == LocationAdd::state)
        return new LocationAdd(doc);
    if(state == LocationDelete::state)
        return new LocationDelete(doc);
    if(state == AccountGet::state)
        return new AccountGet(doc);
    if(state == AccountDel::state)
        return new AccountDel(doc);
    if(state == AccountUpdate::state)
        return new AccountUpdate(doc);
    if(state == MsgInAddAccount::state)
        return new MsgInAddAccount(doc);
    if(state == MsgInAddView::state)
        return new MsgInAddView(doc);
    if(state == MsgInUpdateView::state)
        return new MsgInUpdateView(doc);
    if(state == GetLog::state)
        return new GetLog(doc);
    if(state == MsgInDelView::state)
        return new MsgInDelView(doc);
    if(state == MsgInGetViews::state)
        return new MsgInGetViews(doc);
    if(state == MsgInDelDevice::state)
        return new MsgInDelDevice(doc);
    if(state == MsgInDevices::state)
        return new MsgInDevices(doc);
    if(state == MsgInAdapterListen::state)
        return new MsgInAdapterListen(doc);
    if(state == MsgInSwitch::state)
        return new MsgInSwitch(doc);
    if(state == GateAdd::state)
        return new GateAdd(doc);
    if(state == MsgInGetNewDevices::state)
        return new MsgInGetNewDevices(doc);
    if(state == NotificationSetGCMID::state)
        return new NotificationSetGCMID(doc);
    if(state == NotificationEraseGCMID::state)
        return new NotificationEraseGCMID(doc);
    if(state == UserGetID::state)
        return new UserGetID(doc);
    if(state == UserGetInfo::state)
        return new UserGetInfo(doc);
    if(state == UserLogout::state)
        return new UserLogout(doc);
    if(state == GateGetInfo::state)
        return new GateGetInfo(doc);
    if(state == GateUpdate::state)
        return new GateUpdate(doc); 
    if(state == NotificationsGet::state)
        return new NotificationsGet(doc);
    if(state == NotificationReaded::state)
        return new NotificationReaded(doc);    
    if(state == GateDelete::state)
        return new GateDelete(doc);
    
     vector<string> algMsgs = {"addalg", "getalg", "getalgs", "setalg", "delalg", "getallalgs", "passborder"};
    
    if(std::find(algMsgs.begin(), algMsgs.end(), state)!=algMsgs.end())
        return new MsgInAlgorithmsRedirect(doc);
    
    vector<string> gamiMsgs = {"getallachievements","setprogresslvl"};
    
    if(std::find(gamiMsgs.begin(), gamiMsgs.end(), state) != gamiMsgs.end())
        return new MsgInGamificationRedirect(doc);   

    Logger::getInstance(Logger::ERROR)<<"UNKNOWN MSG"<<endl;
    return new MsgInUnknown(doc);
}
