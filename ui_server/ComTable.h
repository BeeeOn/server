#ifndef COMTABLE_H
#define	COMTABLE_H

#include <map>
#include <string>
#include "../ui_logger/Logger.h"

#include <chrono>
#include <thread>
#include <mutex>

using namespace std;
struct ComTableEntry {
    chrono::time_point<chrono::system_clock> lastActivity;
    time_t t;
    string mail;
    string adapterId;
    int id;
};

struct user {
    int id;
    string mail;
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
    
    int setComInfo(string email);
    int setComInfo(int userId, string email);
    
    bool _cleanerStop;
    int _maxInactivityMs;
    mutex _mtx;
private:
    int getNewComId();
    
    static ComTable* pInstance;
    map<int, ComTableEntry*> _comInfos;
    
    thread * _comTableCleanerThread;
};

#endif	/* COMTABLE_H */