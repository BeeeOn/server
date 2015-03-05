/*
 * config.cpp
 *
 *  Created on: Oct 19, 2014
 *      Author: tuso
 */

#include "config.h"

using namespace pugi;

bool Config::setConfig(std::string File)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(File.c_str());
	switch(result.status)
	{
	  case status_ok:
	  {
	    xml_node ConfRoot = doc.child("configuration");
	    if(ConfRoot == NULL)
	    {
	    	std::cerr<<"Wrong format configuration file !"<<std::endl;
	    	return (false);
	    }
	    bool result =true;
	    xml_node database = ConfRoot.child("Database");
	    if(ConfRoot == NULL)
		{
	    	std::cerr<<"Wrong format configuration file missing database section !"<<std::endl;
	    	result=false;
		}
	    xml_node sender = ConfRoot.child("Sender");
	    if(sender == NULL)
		{
	    	std::cerr<<"Wrong format configuration file missing sender section !"<<std::endl;
	    	result=false;
		}
	    xml_node receiver = ConfRoot.child("Receiver");
		if(receiver == NULL)
		{
			std::cerr<<"Wrong format configuration file missing receiver section !"<<std::endl;
			result=false;
		}
	    this->GetDatabaseProperties(database);
	    this->GetReceiverProperties(receiver);
	    this->GetSenderProperties(sender);
	    return (result);
	    break;
	  }
	  case status_file_not_found:

		  std::cerr<<"Configuration file not found!"<<std::endl;
		  break;
	  case status_io_error:
	  case status_out_of_memory:
	  case status_internal_error:
		  std::cerr<<"Error while opening configuration file !"<<std::endl;
	    break;
	  case status_unrecognized_tag:
	  case  status_bad_pi:
	  case status_bad_comment:
	  case status_bad_cdata:
	  case status_bad_doctype:
	  case status_bad_pcdata:
	  case status_bad_start_element:
	  case status_bad_attribute:
	  case status_bad_end_element:
	  case status_end_element_mismatch:
	  case status_no_document_element:
		  std::cerr<<"XML error occurred while reading configuration file !"<<std::endl;
	    break;
	  default:
		  std::cerr<<"Unknown error occurred while opening/reading configuration file !"<<std::endl;
	    break;
	}
	return (false);
}

bool Config::GetLogProperties(pugi::xml_node log,int *verbosity,int *maxFiles,int*maxLines,std::string *fileNaming,std::string *path)
{
	*verbosity=log.child("Level").text().as_int();
	std::cout<<*verbosity<<"\n";
	*maxLines=log.child("MaxFileSize").text().as_int();
	std::cout<<*maxLines<<"\n";
	*maxFiles=log.child("FilesCount").text().as_int();
	std::cout<<*maxFiles<<"\n";
	*fileNaming=log.child("FileNaming").text().as_string();
	std::cout<<*fileNaming<<"\n";
	*path =log.child("LogPath").text().as_string();
	return (true);
}
bool Config::GetDatabaseProperties (pugi::xml_node DB)
{
	this->_ConnLimit = DB.child("ConnectionsCount").text().as_int();
	this->_DBName = DB.child("Name").text().as_string();
	this->_password = DB.child("Password").text().as_string();
	this->_user = DB.child("User").text().as_string();
	return (true);
}
bool Config::GetReceiverProperties (pugi::xml_node receiver)
{
	this->_receiverPort = receiver.child("Port").text().as_int();
	this->_timeOut = receiver.child("ConnectionTimeOut").text().as_int();
	xml_node log = receiver.child("LogConfig");
	if (log == NULL)
	{
		return (false);
	}
	this->GetLogProperties(log,&(this->_receiverVerbosity),&(this->_receiverMaxFiles),&(this->_receiverMaxLines),&(this->_receiverFileNaming),&(this->_receiverLogPath));
	return (true);
}
bool Config::GetSenderProperties (pugi::xml_node sender)
{
	this->_senderPort = sender.child("Port").text().as_int();
	this->_adapter_port = sender.child("AdapterPort").text().as_int();
	xml_node log = sender.child("LogConfig");
	if (log == NULL)
	{
		return (false);
	}
	this->GetLogProperties(log,&(this->_senderVerbosity),&(this->_senderMaxFiles),&(this->_senderMaxLines),&(this->_senderFileNaming),&(this->_senderLogPath));
	return (true);
}




