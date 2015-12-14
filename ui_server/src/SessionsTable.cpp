#include "SessionsTable.h"
#include "ServerException.h"
#include "Config.h"
#include <chrono>
#include <map>

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <bits/shared_ptr.h>
using namespace std;

SessionsTable::SessionsTable()
{
}

SessionsTable::SessionsTable(int cleanIntervalMs, int maxInactivityMs, bool debug)
    :   _debug(debug)
{ 
    Logger::debug() << "SessionTable starts with max inactivity time: " << maxInactivityMs << endl;
    startSessionsTableCleaner(cleanIntervalMs, maxInactivityMs);
}


SessionsTable::~SessionsTable()
{
    if(_comTableCleanerThread != NULL)
    {
        _cleanerStop = true;
    }
    for (map<string, std::shared_ptr<SessionsTableEntry>>::iterator it=_sessionMap.begin(); it != _sessionMap.end(); ++it)
        it->second.reset();
    _sessionMap.clear();
}


void SessionsTable::cleaner(int sleepTimeMs)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
    
    while(_cleanerStop == false)
    {
        Logger::debug3() <<"Clean com table"<<endl;
        removeOldEntries();  
        printTable();
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
    }
    Logger::debug3() <<"Cleaner done"<<endl;
}

void SessionsTable::startSessionsTableCleaner(int cleanIntervalMs, int maxInactivityMs )
{
    _maxInactivityMs = maxInactivityMs;
    _comTableCleanerThread = unique_ptr<thread>(new thread(& SessionsTable::cleaner, this, cleanIntervalMs));
}

int SessionsTable::removeEntry(std::string sessionString) 
{
    std::lock_guard<std::mutex> lck (_mtx);
    map<string, std::shared_ptr<SessionsTableEntry>>::iterator it;
   
    it = _sessionMap.find(sessionString);
    if(it == _sessionMap.end())
    {
        return -1;
    }
    else
    {
        it->second.reset();
        _sessionMap.erase(it);
        return 0; 
    }
}
void SessionsTable::removeUsersEntries(string userMail)
{
    std::lock_guard<std::mutex> lck (_mtx);
    
    if(_sessionMap.size()==0)
        return;
    

    for ( auto it = _sessionMap.begin(); it != _sessionMap.end(); )
    {
        
        if(it->second->userMail == userMail)
        {
            auto itToDel = it;
            ++it;
            itToDel->second.reset();
            _sessionMap.erase(itToDel);
        }
        else
        {
            ++it;
        }
    }
    
}

void SessionsTable::removeOldEntries()
{
    std::lock_guard<std::mutex> lck (_mtx);
    chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
    //erase every entry with last activity too long ago
    if(_sessionMap.size()==0)
        return;
    
    for ( auto it = _sessionMap.begin(); it != _sessionMap.end(); )
    {
        auto diff = now - it->second->lastActivity;
        
        if(chrono::duration_cast<std::chrono::milliseconds>(diff).count() > _maxInactivityMs)
        {
            auto itToDel = it;
            ++it;
            itToDel->second.reset();
            _sessionMap.erase(itToDel);
        }
        else
        {
            ++it;
        }
    }
    
}

int SessionsTable::getUserIdBySessionString(string sessionString)
{
    std::lock_guard<std::mutex> lck (_mtx);
    int userId = findUserIdBySessionString(sessionString);
    if(userId == -1)
    {
        Logger::getInstance(Logger::DEBUG3) << "User is not logged";
    }
    return userId;
}

int SessionsTable::findUserIdBySessionString(std::string sessionString) 
{
    map<string, std::shared_ptr<SessionsTableEntry>>::iterator it;
    it = _sessionMap.find(sessionString);
    if(it == _sessionMap.end())
    {
        return -1;
    }
    else
    {
        it->second->lastActivity = chrono::system_clock::now();        
        return it->second->userId;
    }
}

std::shared_ptr<SessionsTableEntry> SessionsTable::getEntry(std::string sessionString) 
{
    map<string, std::shared_ptr<SessionsTableEntry>>::iterator it;
   
    it = _sessionMap.find(sessionString);
    if(it == _sessionMap.end())
    {
        return nullptr;
    }
    else
    {
        it->second->lastActivity = chrono::system_clock::now();        
        return it->second;
    }
}

string SessionsTable::addNewSession(int userId, string userMail, string mobileName) 
{
    
    std::lock_guard<std::mutex> lck (_mtx);
    
    string sessionString;
    if(_debug)
        sessionString = "testSession" + userMail;
    else{
        sessionString = getNewSessionString();
    }
    
    std::shared_ptr<SessionsTableEntry> newEntry = make_shared<SessionsTableEntry>();
    newEntry->userId = userId;
    newEntry->mobileName = mobileName;
    newEntry->userMail = userMail;
    newEntry->lastActivity = chrono::system_clock::now();
    
    auto it = _sessionMap.begin();
    
    auto ret = _sessionMap.insert(it,std::pair<string, std::shared_ptr<SessionsTableEntry>>(sessionString, newEntry));

    if (ret != _sessionMap.end()) 
    {
      std::cout << "update session table entry, userid: " << ret->second->userId;
      ret->second = newEntry;
    }
    return sessionString;
}

string SessionsTable::getNewSessionString()
{
    
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    string sessionString;
    int triesToAcquireRandomSource=0;
    do
    {
        ++triesToAcquireRandomSource;
        if(triesToAcquireRandomSource >= 5)
            throw ServerException(ServerException::RANDOMGEN);
        

        int fd = open("/dev/urandom", O_RDONLY);
        if (fd != -1) {
            int len = 10;
            char buffer[len];
            if (read(fd, buffer, sizeof (buffer)) == (int)sizeof (buffer)) 
            {
                
                for (int i = 0; i < len; ++i) 
                {
                    sessionString += alphanum[buffer[i] % (sizeof (alphanum) - 1)];
                }
            }
            else 
            {
                continue;
            }
        }
    }while(findUserIdBySessionString(sessionString) != -1);
    
    return sessionString;
}

void SessionsTable::printTable() 
{
    Logger::debug3() << "sessions: " << _sessionMap.size() << " max. size: " << _sessionMap.max_size() << endl; 
    
    int i=0;
    for(auto it=_sessionMap.begin(); it!=_sessionMap.end(); ++it)
    {
        ++i;
        Logger::debug3() << "entry " << i << ":" << it->first << ", " << it->second->userId <<", " << chrono::time_point_cast<std::chrono::seconds>(it->second->lastActivity).time_since_epoch().count() << endl;    
    };
     Logger::debug3() << "---------- " << endl; 
    
    return;
}
