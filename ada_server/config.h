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
		int _port;
		int _adapter_port;
		int _ConnLimit;
		int _Verbosity;
		std::string _fileNaming;
		int _maxFiles;
		int _maxLines;
		int _timeOut;
		std::string _path;
		bool GetLogProperties(pugi::xml_node log);
	public:
		bool setConfig(std::string File,int AppType);
		std::string DBName(){return this->_DBName;};
		int Port(){return this->_port;};
		int ConnLimit(){return this->_ConnLimit;};
		int Verbosity(){return this->_Verbosity;};
		std::string FileNaming(){return this->_fileNaming;};
		int MaxFiles(){return this->_maxFiles;};
		int MaxLines(){return this->_maxLines;};
		int AdapterPort(){return _adapter_port;};
		int TimeOut(){return _timeOut;};

};


#endif /* CONFIG_H_ */
