#ifndef DBCONNECTOR_H
#define	DBCONNECTOR_H
#include <ctime>
#include <string>
#include <vector>
#include <malloc.h>
#include <iostream>
#include <vector>
#include <soci/soci.h>
#include <soci/postgresql/soci-postgresql.h>
#include "Logger.h"
#include "IMsgIn.h"
#include "gTokenChecker.h"
#include "ServerException.h"




class DBConnector
{
private:
    DBConnector();
    DBConnector(DBConnector const&);// Don't Implement
    void operator=(DBConnector const&); // Don't implement
    void clean();
public:
    int test;
    static DBConnector& getInstance();
    soci::session * getSession();
    void releaseSession(soci::session* session);
    
    ~DBConnector(void);
    void setConnectionStringAndOpenSessions(std::string conString, int sessionPoolSize = 10);
    
    void DEBUGsetConnectionStringAndOpenSessions(std::string conString, int sessionPoolSize = 10);
    void DEBUGrollbackEverything();
    std::string DEBUGexec(std::string sqlQuery);
        
    std::string escapeString(std::string str);
    
    int insertNewUser(std::string gid, googleInfo gInfo);
    int insertNewMobileDevice(IhaToken ihaToken, std::string gId, std::string phoneId, std::string phoneLocale);
    GUserId getUserIdbyIhaToken(IhaToken ihaToken);
    
    int getUserId(std::string email);
    bool isUserRegistred(std::string email);
    string getUserRoleM(GUserId gUserId, std::string adapterId);
    void updatePhoneLocale(std::string mail, std::string phoneLocale);
    std::string getXMLusersAdapters(std::string email);
    bool isAnyUnregistredAdapter();
    bool isUserParredWithAdapter(std::string mail, std::string adapter);
    std::string getXMLDevicesQueryString(std::string facilitiesCond="");
    std::string getXMLAllDevs(std::string adapter);
    std::string getXMLdevices(GUserId userId, vector<std::string> adaptersVec, vector<device> devicesVec);
    std::string getXMLNewDevices(std::string adapterId);
    bool isAdapterRegistrable(std::string adapterId);
    int parAdapterWithUserIfPossible(int adapterId, std::string adapterName, std::string gId);
    void insertNewUserParredWithUnregistredAdapter(std::string adapterId, std::string mail, bool registerNewUser = false);
    
    /*ROOMS*/
    int insertNewRoom(string adapterId, string roomType, string roomName);
    void updateRoom(string adapterId, string roomId, string type, string name);
    void deleteRoom(string adapterId, string roomId);
    
    string getXMLrooms(string adapterId);
    string getUserId(soci::session & sql,string userMail);
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
    string insertNewAction(GUserId userId, string actionName, string actionXml);
    string getActionsList(GUserId userId);  
    string getAction(string actionId);
    int updateAction(string actionId, string actionName, string actionXml) ;
    void deleteAction(string actionId) ;
    
    int connectConditionWithAction(string condId, string actionId) ;
    
    /*Google*/
    int updateUsersGCMID(GUserId userId, string gcmid);
    int updateUserGoogleInformation(GUserId userId, googleInfo gInfo);
    
    /*NOTIFICATION*/
    int setGCMId(IhaToken IHAtoken, string phoneid, string gUserId, string gcmid); 
    int delGCMId(string oldUserId, string gcmid);
private:
    string _connectionString;
    size_t _poolSize;
    soci::connection_pool* _pool;
};

#endif /* DBCONNECTOR_H*/

