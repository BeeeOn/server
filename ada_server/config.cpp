/**
 * @file config.cpp
 * 
 * @brief implementation of Config Class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "config.h"
#include <Poco/AutoPtr.h>
#include <Poco/Exception.h>
#include <Poco/Util/AbstractConfiguration.h>
#include <Poco/Util/IniFileConfiguration.h>
#include <Poco/Util/XMLConfiguration.h>

using namespace Poco;
using namespace Poco::Util;

static bool parseConfBool(AbstractConfiguration *cfg, const char *key, bool def)
{
	std::string value = cfg->getString(key, def? "true" : "false");
	if (!value.compare("true") || !value.compare("t"))
		return true;

	if (!value.compare("false") || !value.compare("f"))
		return false;

	return cfg->getBool(key, def);
}

bool Config::parseConfig(AbstractConfiguration *cfg)
{
	this->_ConnLimit = cfg->getInt("Database.ConnectionsCount");
	this->_DBName = cfg->getString("Database.Name");
	this->_password = cfg->getString("Database.Password");
	this->_user = cfg->getString("Database.User");

	this->_receiverPort = cfg->getInt("Receiver.Port");
	this->_timeOut = cfg->getInt("Receiver.ConnectionTimeOut");

	this->_receiverVerbosity = cfg->getInt("Receiver.LogConfig.Level", 7);
	if (parseConfBool(cfg, "Receiver.LogConfig.ToSTD", true)) {
		this->_receiverMaxFiles = cfg->getInt("Receiver.LogConfig.FilesCount");
		this->_receiverMaxLines = cfg->getInt("Receiver.LogConfig.MaxFileSize");
		this->_receiverFileNaming = cfg->getString("Receiver.LogConfig.FileNaming");
		this->_receiverLogPath = cfg->getString("Receiver.LogConfig.LogPath");
	}

	this->_senderPort = cfg->getInt("Sender.Port");
	this->_adapter_port = cfg->getInt("Sender.AdapterPort");

	this->_senderVerbosity = cfg->getInt("Sender.LogConfig.Level", 7);
	if (parseConfBool(cfg, "Sender.LogConfig.ToSTD", true)) {
		this->_senderMaxFiles = cfg->getInt("Sender.LogConfig.FilesCount");
		this->_senderMaxLines = cfg->getInt("Sender.LogConfig.MaxFileSize");
		this->_senderFileNaming = cfg->getString("Sender.LogConfig.FileNaming");
		this->_senderLogPath = cfg->getString("Sender.LogConfig.LogPath");
	}

	this->_mode = cfg->getString("Common.Mode").compare("DEBUG");
	this->_CApath = cfg->getString("Common.Certificates.CACrtFile");
	this->_crtPath = cfg->getString("Common.Certificates.CrtFile");
	this->_keyPath = cfg->getString("Common.Certificates.KeyFile");

	return true;
}

bool Config::setConfig(std::string file)
{
	AutoPtr<XMLConfiguration> cfg(new XMLConfiguration(file));
	return parseConfig(cfg);
}
