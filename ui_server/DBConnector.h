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
#include "Logger.h"
#include "IMsgIn.h"
#include "gTokenChecker.h"
#include "ServerException.h"


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
    int test;
    static DBConnector& getInstance();
    
    ~DBConnector(void);
    void setConnectionStringAndOpenSessions(string conString, int sessionPoolSize = 10);
    
    void DEBUGsetConnectionStringAndOpenSessions(string conString, int sessionPoolSize = 10);
    void DEBUGrollbackEverything();
    string DEBUGexec(string sqlQuery);
        
    string escapeString(string str);
    int getUserId(string email);
    bool isUserRegistred(string email);
    string getUserRole(string userMail, string adapterId);
    void updatePhoneLocale(string mail, string phoneLocale);
    string getXMLusersAdapters(string email);
    bool isAnyUnregistredAdapter();
    bool isUserParredWithAdapter(string mail, string adapter);
    string getXMLDevicesQueryString(string facilitiesCond="");
    string getXMLAllDevs(string adapter);
    string getXMLdevices(int userId, vector<device> devicesVec);
    string getXMLNewDevices(string adapterId);
    bool isAdapterRegistrable(string adapterId);
    int insertNewUser(string mail);
    int parAdapterWithUserIfPossible(int adapterId, string adapterName, string mail);
    void insertNewUserParredWithUnregistredAdapter(string adapterId, string mail, bool registerNewUser = false);
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
    string insertNewAction(int _parredUserId, string actionName, string actionXml);
    string getActionsList(int userld);  
    string getAction(string actionId);
    int updateAction(string actionId, string actionName, string actionXml) ;
    void deleteAction(string actionId) ;
    
    int connectConditionWithAction(string condId, string actionId) ;
    
    /*Google*/
    int updateUsersGCMID(int userId, string gcmid);
    int updateUserGoogleInformation(int userId, googleInfo gInfo);
private:
    string _connectionString;
    size_t _poolSize;
    soci::connection_pool* _pool;
};

#endif /* DBCONNECTOR_H*/
