/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MessageParser.h
 * Author: mrmaidx
 *
 * Created on 20. ledna 2016, 14:12
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

