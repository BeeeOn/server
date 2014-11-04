/* 
 * File:   MsgRightsChecker.h
 * Author: pavel
 *
 * Created on 3. červenec 2014, 12:47
 */

#ifndef MSGRIGHTSCHECKER_H
#define	MSGRIGHTSCHECKER_H

#include <string>
#include <iostream>
#include <map>



using namespace std; 

class IMsgInLoginAndAdapterAccessRequired;

class MsgRightsChecker {
private:
    MsgRightsChecker();
    MsgRightsChecker(MsgRightsChecker const&);              // Don't Implement
    void operator=(MsgRightsChecker const&); // Don't implement
public:
    virtual ~MsgRightsChecker();

    static MsgRightsChecker& getInstance();
    bool checkRights(IMsgInLoginAndAdapterAccessRequired& msg);
    
private:
    map<string,int> _msgRightsTable;
    map<string,int> _rightsTable;
};

#endif	/* MSGRIGHTSCHECKER_H */

