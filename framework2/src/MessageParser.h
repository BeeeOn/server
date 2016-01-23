/* 
 * File:   MessageParser.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 20. January 2016
 */

#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H



class MessageParser {
public:
    MessageParser();
    MessageParser(const MessageParser& orig);
    virtual ~MessageParser();
    
    static void parseMessage();
    
private:

};

#endif /* MESSAGEPARSER_H */

