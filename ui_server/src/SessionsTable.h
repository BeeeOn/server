#ifndef SESSIONSTABLE_H
#define	SESSIONSTABLE_H

#include <map>
#include <string>
#include "../ui_logger/Logger.h"
#include "../DAO/DAOUsers.h"
#include <chrono>
#include <thread>
#include <mutex>

struct SessionsTableEntry {
    std::string mobileName;
    std::string locale;
    std::string token;
    std::chrono::time_point<std::chrono::system_clock> lastActivity;
    std::string userMail;
    int userId;
};

class SessionsTable
{
public:
    SessionsTable();
    SessionsTable(int cleanIntervalMs, int maxInactivityMs, bool debug = false);
    ~SessionsTable(void);
    
    void startSessionsTableCleaner(int cleanIntervalMs, int maxInactivityMs);
    int removeEntry(std::string sessionString);
    void removeUsersEntries(std::string userMail);
    void removeOldEntries();
    int getUserIdBySessionString(std::string sessionString);
    std::shared_ptr<SessionsTableEntry> getEntry(std::string sessionString);
    std::string addNewSession(int userId, std::string userMail, std::string mobileName);
    
    void printTable();
    void test();
    
    bool _cleanerStop = false;
    int _maxInactivityMs;
    std::mutex _mtx;
    
private:
    std::string getNewSessionString();
    int findUserIdBySessionString(std::string sessionString);
    void cleaner(int sleepTimeMs);
    
    std::map<std::string, std::shared_ptr<SessionsTableEntry>> _sessionMap;
    std::unique_ptr<std::thread> _comTableCleanerThread;
    bool _debug = false;
};




#endif	/* SESSIONSTABLE_H */

