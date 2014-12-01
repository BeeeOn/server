/*
 * config.cpp
 *
 *  Created on: Oct 19, 2014
 *      Author: tuso
 */

#include "config.h"

using namespace pugi;

bool Config::setConfig(std::string File,int AppType)
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
	    	return false;
	    }
	    this->_ConnLimit=ConfRoot.child("ConnectionsCount").text().as_int();
	    this->_DBName=ConfRoot.child("Database").text().as_string();
	    this->_port=ConfRoot.child("Port").text().as_int();
	    if (AppType==1)
	    {
	    	this->_adapter_port=ConfRoot.child("AdapterPort").text().as_int();
	    }
	    else
	    {
	    	this->_timeOut=ConfRoot.child("ConnectionTimeOut").text().as_int();
	    }
	    xml_node Log = ConfRoot.child("LogConfig");
	    this->GetLogProperties(Log);
	    return true;
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
	return false;
}

bool Config::GetLogProperties(xml_node Log)
{
	this->_Verbosity=Log.child("Level").text().as_int();
	this->_maxLines=Log.child("MaxFileSize").text().as_int();
	this->_maxFiles=Log.child("FilesCount").text().as_int();
	this->_fileNaming=Log.child("FileNaming").text().as_string();
	this->_path =Log.child("LogPath").text().as_string();
	return true;
}




