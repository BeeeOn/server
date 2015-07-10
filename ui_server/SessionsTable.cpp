/* 
 * File:   SessionsTable.cpp
 * Author: Pavel
 * 
 * Created on 10. července 2015, 9:49
 */

#include "SessionsTable.h"
#include "ServerException.h"
#include <chrono>
#include <map>


#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
using namespace std;

SessionsTable::SessionsTable(){
    
    _comTableCleanerThread = NULL;
    _cleanerStop = false;
}


SessionsTable::~SessionsTable()
{
    if(_comTableCleanerThread != NULL){
        _cleanerStop = true;
        /*if(_comTableCleanerThread->joinable())
        _comTableCleanerThread->join();
        delete _comTableCleanerThread;*/
    }
    for (map<string, SessionsTableEntry*>::iterator it=_sessionMap.begin(); it!=_sessionMap.end(); ++it)
        delete it->second;
    _sessionMap.clear();
}

SessionsTable& SessionsTable::getInstance(){
    static SessionsTable instance;
    return instance;
}

void cleaner(int sleepTimeMs){
    SessionsTable & sessionsTable = SessionsTable::getInstance();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
    
    while(sessionsTable._cleanerStop == false){
        Logger::debug3() <<"Clean com table"<<endl;
        sessionsTable.removeOldEntries();       
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
    }
    Logger::debug3() <<"Cleaner done"<<endl;
}

void SessionsTable::startSessionsTableCleaner(int cleanIntervalMs, int maxInactivityMs ){
    _maxInactivityMs = maxInactivityMs;
    _comTableCleanerThread = new thread(cleaner, cleanIntervalMs);
}

int SessionsTable::removeEntry(std::string sessionString) {
    std::lock_guard<std::mutex> lck (_mtx);
    map<string, SessionsTableEntry*>::iterator it;
   
    it = _sessionMap.find(sessionString);
    if(it == _sessionMap.end()){
        return -1;
    }else{
        delete it->second;
        _sessionMap.erase(it);
        return 0; 
    }
}

void SessionsTable::removeOldEntries(){
    std::lock_guard<std::mutex> lck (_mtx);
    chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
    //erase every entry with last activity too long ago
    if(_sessionMap.size()==0)
        return;
    
    map<string, SessionsTableEntry*>::iterator it;
    map<string, SessionsTableEntry*>::iterator itToDel;
    for ( it = _sessionMap.begin(); it != _sessionMap.end(); ){
        auto diff = now - it->second->lastActivity;
        //cout<<chrono::duration_cast<std::chrono::milliseconds>(diff).count() <<endl;
        
        if(chrono::duration_cast<std::chrono::milliseconds>(diff).count() > _maxInactivityMs){
            itToDel = it;
            it++;
            delete itToDel->second;
            _sessionMap.erase(itToDel);
        }else{
            it++;
        }
    }
    
}

int SessionsTable::getUserIdBySessionString(string sessionString)
{
    std::lock_guard<std::mutex> lck (_mtx);
    int userId = findUserIdBySessionString(sessionString);
    if(userId == -1)
        Logger::getInstance(Logger::DEBUG3) << "User is not logged";
    return userId;
}

int SessionsTable::findUserIdBySessionString(std::string sessionString) {
    map<string, SessionsTableEntry*>::iterator it;
   
    it = _sessionMap.find(sessionString);
    if(it == _sessionMap.end()){
        return -1;
    }else{
        it->second->lastActivity = chrono::system_clock::now();
        
        return it->second->id;
    }
}


int SessionsTable::addNewSession(int userId) {
    std::lock_guard<std::mutex> lck (_mtx);
    string sessionString = getNewSessionString();
    SessionsTableEntry* e = new SessionsTableEntry;
    e->id = userId;
    e->lastActivity = chrono::system_clock::now();
    
    map<string, SessionsTableEntry*>::iterator it = _sessionMap.begin();
    _sessionMap.insert(it,std::pair<string, SessionsTableEntry*>(sessionString,e));
    
    return 1;
}

string SessionsTable::getNewSessionString(){
    
    /*static int i=1;
    return i++;*/
    
    string sessionString;
    int i=0;
    do{
        i++;
        if(i >= 5)
            throw ServerException(ServerException::RANDOMGEN);
        
        

        static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";

        int fd = open("/dev/urandom", O_RDONLY);
        if (fd != -1) {
            // uint8_t buffer[len];
            int len = 10;
            char buffer[len];
            if (read(fd, buffer, sizeof (buffer)) == (int)sizeof (buffer)) {
                
                for (int i = 0; i < len; ++i) {
                    sessionString += alphanum[buffer[i] % (sizeof (alphanum) - 1)];
                }
            } else {
                continue;
            }
        }
        
     /*   ifstream urandom("/dev/random", ios::binary);
        if(urandom.is_open())
            cerr <<"!!!!!!!"<<endl;
        urandom.read(block,len);
        urandom.close();
     
        
        for (int i = 0; i < len; ++i) {
            newToken += alphanum[block[i] % (sizeof(alphanum) - 1)];  
        }
 */
        //r = DBConnector::getInstance().DEBUGexec("select  count(token)  from mobile_devices where token='"+sessionString+"'" );
        
    }while(findUserIdBySessionString(sessionString) != -1);
    
    return sessionString;
}

void SessionsTable::printTable() {
    Logger::debug3() << "sessions: " << _sessionMap.size() << " max. size: " << _sessionMap.max_size() << endl; 
    
    int i=0;
    for (map<string, SessionsTableEntry*>::iterator it=_sessionMap.begin(); it!=_sessionMap.end(); ++it)
    {
        i++;
        Logger::debug3() << "entry " << i << ":" << it->first << ", " << it->second->id << endl;    
    };
     Logger::debug3() << "---------- " << endl; 
    
    return;
}

void SessionsTable::test() {
    int i=0;
    map<string, SessionsTableEntry*>::iterator itToDel;
    for (map<string, SessionsTableEntry*>::iterator it=_sessionMap.begin(); it!=_sessionMap.end();)
    {
        i++;
         
        if(i==1){
            Logger::debug3() << "del " << i <<endl;
            itToDel = it;
            it++;
            delete itToDel->second;
            _sessionMap.erase(itToDel);
            
        }else{
            it++;
        }
    };
}
