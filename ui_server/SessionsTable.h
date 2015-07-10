/* 
 * File:   SessionsTable.h
 * Author: Pavel
 *
 * Created on 10. července 2015, 9:49
 */

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
    std::chrono::time_point<std::chrono::system_clock> lastActivity;
    time_t t;
    std::string mail;
    std::string adapterId;
    int id;
};

class SessionsTable
{
private:
    SessionsTable();
    SessionsTable(SessionsTable const&); // Don't Implement
    void operator=(SessionsTable const&); // Don't implement
public:
    ~SessionsTable(void);
    static SessionsTable&  getInstance();
    
    void startSessionsTableCleaner(int cleanIntervalMs, int maxInactivityMs);
    int removeEntry(std::string sessionString);
    void removeOldEntries();
    int getUserIdBySessionString(std::string sessionString);
    
    int addNewSession(int userId);
    
    void printTable();
    void test();
    
    bool _cleanerStop;
    int _maxInactivityMs;
    std::mutex _mtx;
    
private:
    std::string getNewSessionString();
    int findUserIdBySessionString(std::string sessionString);
    
    static SessionsTable* pInstance;
    //map - sessionString - userId
    std::map<std::string, SessionsTableEntry*> _sessionMap;
    std::thread* _comTableCleanerThread;
};




#endif	/* SESSIONSTABLE_H */

