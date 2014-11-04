#include <chrono>

#include "ComTable.h"
#include "DBConnector.h"

ComTable::ComTable(){
    
    _comTableCleanerThread = NULL;
    _cleanerStop = false;
}


ComTable::~ComTable()
{
    if(_comTableCleanerThread != NULL){
        _cleanerStop = true;
        _comTableCleanerThread->detach();
        delete _comTableCleanerThread;
    }
        for (map<int, ComTableEntry*>::iterator it=_comInfos.begin(); it!=_comInfos.end(); ++it)
            delete it->second;
}

ComTable& ComTable::getInstance(){
    static ComTable instance;
    return instance;
}

void cleaner(int sleepTimeMs){
    ComTable & comTable = ComTable::getInstance();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
    
    while(comTable._cleanerStop == false){
        Logger::getInstance(Logger::DEBUG3) <<"Clean com table"<<endl;
        comTable.removeOldEntries();       
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeMs));
    }
}

void ComTable::startComTableCleaner(int cleanIntervalMs, int maxInactivityMs ){
    _maxInactivityMs = maxInactivityMs;
    _comTableCleanerThread = new thread(cleaner, cleanIntervalMs);
}

void ComTable::removeOldEntries(){
    std::lock_guard<std::mutex> lck (_mtx);
    chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
    //erase every entry with last activity too long ago
    if(_comInfos.size()==0)return;
    
    map<int, ComTableEntry*>::iterator it;
    
    for ( it=_comInfos.begin(); it!=_comInfos.end(); ){
        auto diff =now - it->second->lastActivity;
        //cout<<chrono::duration_cast<std::chrono::milliseconds>(diff).count() <<endl;
        
        if(chrono::duration_cast<std::chrono::milliseconds>(diff).count() > _maxInactivityMs){
            delete it->second;
            it = _comInfos.erase(it);
        }else{
            it++;
        }
    }
    
}

user ComTable::getUserByComId(int id)
{
    std::lock_guard<std::mutex> lck (_mtx);
    map<int, ComTableEntry*>::iterator it;
    user u;
    u.mail= "";
    
    it = _comInfos.find(id);
    if(it == _comInfos.end()){
        Logger::getInstance(Logger::DEBUG3) << "User is not logged";
        return u;
    }else{
        it->second->lastActivity = chrono::system_clock::now();
        
        u.id= it->second->id;
        u.mail = it->second->mail;
                
        return u;
    }
}
int ComTable::setComInfo( string email)
{
    //TODO DEBUG jenom pro testovani !!!
    int userId = DBConnector::getInstance().getUserId(email);
    
    std::lock_guard<std::mutex> lck (_mtx);
    int comId = getNewComId();
    ComTableEntry* e = new ComTableEntry;
    e->id = userId;
    e->mail= email;
    e->lastActivity = chrono::system_clock::now();
    
    map<int, ComTableEntry*>::iterator it = _comInfos.begin();
    _comInfos.insert(it,std::pair<int, ComTableEntry*>(comId,e));
    
    return comId;
}
int ComTable::setComInfo(int userId, string email)
{
    std::lock_guard<std::mutex> lck (_mtx);
    int comId = getNewComId();
    ComTableEntry* e = new ComTableEntry;
    e->id = userId;
    e->mail= email;
    e->lastActivity = chrono::system_clock::now();
    
    map<int, ComTableEntry*>::iterator it = _comInfos.begin();
    _comInfos.insert(it,std::pair<int, ComTableEntry*>(comId,e));
    
    return comId;
}



int ComTable::getNewComId(){
    
    static int i=1;
    return i++;
}
