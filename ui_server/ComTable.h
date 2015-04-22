#ifndef COMTABLE_H
#define	COMTABLE_H

#include <map>
#include <string>
#include "../ui_logger/Logger.h"

#include <chrono>
#include <thread>
#include <mutex>

struct ComTableEntry {
    std::chrono::time_point<std::chrono::system_clock> lastActivity;
    time_t t;
    std::string mail;
    std::string adapterId;
    int id;
};

struct user {
    int id;
    std::string mail;
};

class ComTable
{
private:
    ComTable();
    ComTable(ComTable const&);              // Don't Implement
    void operator=(ComTable const&); // Don't implement
public:
    ~ComTable(void);
    static ComTable&  getInstance();
    
    void startComTableCleaner(int cleanIntervalMs, int maxInactivityMs);
    void removeOldEntries();
    user getUserByComId(int id);
    
    int setComInfo(std::string email);
    int setComInfo(int userId, std::string email);
    
    bool _cleanerStop;
    int _maxInactivityMs;
    std::mutex _mtx;
private:
    int getNewComId();
    
    static ComTable* pInstance;
    std::map<int, ComTableEntry*> _comInfos;
    
    std::thread * _comTableCleanerThread;
};

#endif	/* COMTABLE_H */