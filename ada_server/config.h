/*
 * config.h
 *
 *  Created on: Oct 19, 2014
 *      Author: tuso
 */

#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include <iostream>

#ifndef CONFIG_H_
#define CONFIG_H_


class Config
{
	private:
		std::string _DBName;
		int _senderPort;
		int _receiverPort;
		int _adapter_port;
		int _ConnLimit;
		int _senderVerbosity;
		int _receiverVerbosity;
		std::string _receiverFileNaming;
		std::string _senderFileNaming;
		int _senderMaxFiles;
		int _senderMaxLines;
		int _receiverMaxFiles;
		int _receiverMaxLines;
		std::string _receiverLogPath;
		std::string _senderLogPath;
		int _timeOut;
		std::string _path;
		std::string _password;
		std::string _user;
		bool GetLogProperties(pugi::xml_node log,int *verbosity,int *maxFiles,int*maxLines,std::string *fileNaming,std::string *path);
		bool GetDatabaseProperties (pugi::xml_node DB);
		bool GetReceiverProperties (pugi::xml_node receiver);
		bool GetSenderProperties (pugi::xml_node sender);
	public:
		bool setConfig(std::string File);
		std::string DBName() const {return (this->_DBName);};
		int SenderPort() const {return (this->_senderPort);};
		int ReceiverPort() const {return (this->_receiverPort);};
		int AdapterPort() const {return (_adapter_port);};
		int ConnLimit() const {return (this->_ConnLimit);};
		int SenderVerbosity() const {return (this->_senderVerbosity);};
		std::string SenderFileNaming() const {return (this->_senderFileNaming);};
		int SenderMaxFiles() const {return( this->_senderMaxFiles);};
		int SenderMaxLines() const {return( this->_senderMaxLines);};
		int ReceiverVerbosity() const {return (this->_receiverVerbosity);};
		std::string ReceiverFileNaming() const {return (this->_receiverFileNaming);};
		int ReceiverMaxFiles() const {return( this->_receiverMaxFiles);};
		int ReceiverMaxLines() const {return( this->_receiverMaxLines);};
	int TimeOut() {
		return (_timeOut);
	}

	;

};


#endif /* CONFIG_H_ */
