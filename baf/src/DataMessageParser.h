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
	/**
	 * Constructor of class DataMessageParser.
	 */
	DataMessageParser();
	/**
	 * Destructor of class DataMessageParser.
	 */
	virtual ~DataMessageParser();
	/**
	 * Parses received XML sensory data message to DataMessage object.
	 * @param received_data String with XML senzory data message.
	 * @return Parsed DataMessage object.
	 */
	DataMessage parseMessage(std::string received_data);
};

#endif /* MESSAGEPARSER_H */
