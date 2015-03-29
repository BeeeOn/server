#ifndef DBCONNECTOR_H
#define	DBCONNECTOR_H
#include <ctime>
#include <string>
#include <vector>
#include <malloc.h>
#include <iostream>
#include <vector>
#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include "../ui_logger/Logger.h"
#include "IMsgIn.h"
#include "gTokenChecker.h"
#include "ServerException.h"
#include "../DAO/DAODevices.h"

using namespace std;
using namespace soci;

class DBConnector
{
private:
    DBConnector();
    DBConnector(DBConnector const&);// Don't Implement
    void operator=(DBConnector const&); // Don't implement
    void clean();
public:
    static DBConnector& getInstance();
    soci::session * getSession();
    void releaseSession(soci::session* session);
    
    ~DBConnector(void);
    void setConnectionStringAndOpenSessions(string conString, int sessionPoolSize = 10);
    
    void DEBUGsetConnectionStringAndOpenSessions(string conString, int sessionPoolSize = 10);
    void DEBUGrollbackEverything();
    string DEBUGexec(string sqlQuery);
        
    string escapeString(string str);
    
    int insertNewUser(string gid, googleInfo gInfo);
    int insertNewMobileDevice(std::string ihaToken, string gId, string phoneId, string phoneLocale);
    string getUserIdbyIhaToken(string ihaToken);
    
    int getUserId(string email);
    bool isUserRegistred(string email);
    string getUserRoleM(int userId, string adapterId);
    void updatePhoneLocale(string mail, string phoneLocale);
    string getXMLusersAdapters(int userId);
    bool isAnyUnregistredAdapter();
    bool isUserParredWithAdapter(string mail, string adapter);
    string getXMLDevicesQueryString(string facilitiesCond="");
    string getXMLAllDevs(string adapter);
    string getXMLdevices(int userId, vector<string> adaptersVec, vector<device> devicesVec);
    string getXMLNewDevices(string adapterId);
    bool isAdapterRegistrable(string adapterId);
    int parAdapterWithUserIfPossible(long long int adapterId, string adapterName, string gId);
    void insertNewUserParredWithUnregistredAdapter(string adapterId, string mail, bool registerNewUser = false);
    
    /*ROOMS*/
    int insertNewRoom(string adapterId, string roomType, string roomName);
    void updateRoom(string adapterId, string roomId, string type, string name);
    void deleteRoom(string adapterId, string roomId);
    
    string getXMLrooms(string adapterId);
    string getUserId(session & sql,string userMail);
    /*DEVICES*/
    int updateFacility(string adapterId,string id,string init, string locationId, string refresh);
    int updateDevice(string adapterId, string id, string type, string name, string visibility);
    string getXMLDeviceLog(string adapterId, device deviceId, string logFrom, string logTo, string dataType, string interval);
    /*TIMEZONES*/
    void updateAdaptersTimezone(string adapterId,  string newTimeZone);
    string getTimeZone(string adapterId);
    /*ACCOUNT*/
    string getXMLconAccounts(string adapterId);
    int delConAccount(string adapterId, string userMail);
    int changeConAccount(string adapterId, string userMail, string newRole);
    int addConAccount(string adapterId, string userMail, string newRole);
    /*VIEWS*/
    void addView(string userMail, string viewName, string viewIcon);
    void addDeviceToView(string  viewName, string  userMail, string deviceId, string deviceType);
    void removeDeviceFromView(string  viewName, string  userMail, string deviceId, string deviceType);
    void delView(string viewName, string userMail);
    string viewsList(string userMail);
    /*CONDITIONS*/
    string insertNewCondition(string mail, string condName, string condType, string condXml);
    string getCondition(string condId);
    string getConditionsList(string mail);
    int updateCondition(string condId, string condName, string condType, string condXml);
    void deleteCondition(string condId);
    /*ACTIONS*/
    string insertNewAction(string userId, string actionName, string actionXml);
    string getActionsList(string userId);  
    string getAction(string actionId);
    int updateAction(string actionId, string actionName, string actionXml) ;
    void deleteAction(string actionId) ;
    
    int connectConditionWithAction(string condId, string actionId) ;
    
    /*Google*/
    int updateUsersGCMID(string userId, string gcmid);
    int updateUserGoogleInformation(string userId, googleInfo gInfo);
    
    /*NOTIFICATION*/
    int setGCMId(string IHAtoken, string phoneid, int gUserId, string gcmid); 
    int delGCMId(string oldUserId, string gcmid);
private:
    string _connectionString;
    size_t _poolSize;
    soci::connection_pool* _pool;
};

#endif /* DBCONNECTOR_H*/

