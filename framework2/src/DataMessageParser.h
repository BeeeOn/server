/* 
 * File:   MessageParser.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include "DataMessage.h"

class DataMessageParser {
public:
    DataMessageParser();
    DataMessageParser(const DataMessageParser& orig);
    virtual ~DataMessageParser();
    
    DataMessage parseMessage(std::string received_data);
    
private:

};

#endif /* MESSAGEPARSER_H */

