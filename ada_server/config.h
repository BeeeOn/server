/**
 * @file config.h
 * 
 * @brief definition of Config Class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include "../lib/pugixml.hpp"
#include "../lib/pugiconfig.hpp"
#include <iostream>

#ifndef CONFIG_H_
#define CONFIG_H_

/** @Class Config
 *  @brief Class parsing XML config file and storing data
 */

class Config
{
	private:
		std::string _DBName; /**< Member representing database name*/
		int _senderPort; /**< Member representing Sender port*/
		int _receiverPort; /**< Member representing Receiver port*/
		int _adapter_port; /**< Member representing adapter port*/
		int _ConnLimit;  /**< Member representing Database connections limit*/
		int _senderVerbosity;  /**< Member representing Sender log level*/
		int _receiverVerbosity; /**< Member representing Receiver log level*/
		std::string _receiverFileNaming; /**< Member representing Receiver file naming*/
		std::string _senderFileNaming; /**< Member representing Sender file naming*/
		int _senderMaxFiles; /**< Member representing Sender history files count*/
		int _senderMaxLines; /**< Member representing Sender max file size*/
		int _receiverMaxFiles; /**< Member representing receiver history files count */
		int _receiverMaxLines; /**< Member representing receiver max */
		std::string _receiverLogPath; /**< Member representing path where to save receiver logs*/
		std::string _senderLogPath; /**< Member representing path where to save sender logs*/
		int _timeOut;  /**< Member representing timeout for unresponsive clients*/
		std::string _password;  /**< Member representing password to database*/
		std::string _user;  /**< Member representing user name to database*/
		std::string _CApath; /**< Member representing path to CA certificate file*/
		std::string _crtPath; /**< Member representing path to server public certificate file*/
		std::string _keyPath; /**< Member representing path to server private key file*/
		int _mode;
		/**Method parse configuration file
		 * @param File pugi::xml_node common element of file
		 * @return bool type representing success/failure of parsing*/
		bool GetCommonProperties(pugi::xml_node Common);
		/**Method to parse configuration file
		 * @param File pugi::xml_node certificates element of file
		 * @return bool type representing success/failure of parsing*/
		bool GetCertificatesProperties(pugi::xml_node Certificates);
		/**Method parse configuration file
		 * @param File pugi::xml_node log element of file
		 * @param verbosity pointer to integer where is saved verbosity setting
		 * @param maxFiles pointer to integer where is saved max Files setting
		 * @param fileNaming pointer to std::string where is saved file naming setting
		 * @param path pointer to integer where is saved path setting
		 * @return bool type representing success/failure of parsing*/
		bool GetLogProperties(pugi::xml_node log,int *verbosity,int *maxFiles,int*maxLines,std::string *fileNaming,std::string *path);
		/**Method to load and parse configuration file
		 * @param File pugi::xml_node DB element of file
		 * @return bool type representing success/failure of parsing*/
		bool GetDatabaseProperties (pugi::xml_node DB);
		/**Method parse configuration file
		 * @param File pugi::xml_node receiver element of file
		 * @return bool type representing success/failure of parsing*/
		bool GetReceiverProperties (pugi::xml_node receiver);
		/**Method to parse configuration file
		 * @param File pugi::xml_node sender element of file
		 * @return bool type representing success/failure of parsing*/
		bool GetSenderProperties (pugi::xml_node sender);
	public:
		/**Method to load and parse configuration file
		 * @param File std::string with name of configuration file
		 * @return bool type representing success/failure of parsing*/
		bool setConfig(std::string File);
		/**Getter for database name from config file
		 * @return std::string representation of database name*/
		std::string DBName() const {return (this->_DBName);};
		/**Getter for Sender part port to listen on
		 * @return int representing port number*/
		int SenderPort() const {return (this->_senderPort);};
		/**Getter for Receiver part to listen on
		 * @return number of port*/
		int ReceiverPort() const {return (this->_receiverPort);};
		/**Getter for adapter port to send message
		 * @return int representing number of port*/
		int AdapterPort() const {return (_adapter_port);};
		/**Getter for database connections limit from configuration
		 * @return int representing number of connections to database*/
		int ConnLimit() const {return (this->_ConnLimit);};
		/**Getter for logger level setting of Sender
		 * @return int representing number of level*/
		int SenderVerbosity() const {return (this->_senderVerbosity);};
		/**Getter for log file names of Sender loger
		 * @return std::string containing file name*/
		std::string SenderFileNaming() const {return (this->_senderFileNaming);};
		/**Getter for length of history of sender logging
		 * @return int containing number of files in history*/
		int SenderMaxFiles() const {return( this->_senderMaxFiles);};
		/**Getter for megabytes limit of log file for sender
		 * @return int representing number of megabytes*/
		int SenderMaxLines() const {return( this->_senderMaxLines);};
		/**Getter for logger level setting of Receiver
		 * @return int representing number of level*/
		int ReceiverVerbosity() const {return (this->_receiverVerbosity);};
		/**Getter for log file names of Receiver loger
		 * @return std::string containing file name*/
		std::string ReceiverFileNaming() const {return (this->_receiverFileNaming);};
		/**Getter for length of history of Receiver logging
		 * @return int containing number of files in history*/
		int ReceiverMaxFiles() const {return( this->_receiverMaxFiles);};
		/**Getter for megabytes limit of log file for Receiver
		 * @return int representing number of megabytes*/
		int ReceiverMaxLines() const {return( this->_receiverMaxLines);};
		/**Getter for path where to save log files of Sender
		 * @return std::string containing path*/
		std::string SenderPath() const {return (this->_senderLogPath);};
		/**Getter for path where to save log files of Receiver
		 * @return std::string containing path*/
		std::string ReceiverPath() const {return (this->_receiverLogPath);};
		/**Getter for database password
		 * @return std::string containing password*/
		std::string Password() const {return (this->_password);};
		/**Getter for database user
		 * @return std::string containing user*/
		std::string User() const {return (this->_user);};
		/**Getter for timeout setting
		 * @return int number for seconds of timeout*/
		int TimeOut() {return (_timeOut);};
		/**Getter CA certificate path
		 * @return std::string containing path to file*/
		std::string CApath ()const {return (_CApath);};
		/**Getter server public certificate path
		 * @return std::string containing path to file*/
		std::string CRTPath () const {return (_crtPath);};
		/**Getter server private key path
		 * @return std::string containing path to file*/
		std::string KeyPath () const {return (_keyPath);};
		/**Getter for server mode to start in
		 * @return int representing mode number*/
		int Mode () const {return (_mode);};

};


#endif /* CONFIG_H_ */
