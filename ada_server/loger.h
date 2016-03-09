/**
 * @file loger.h
 * 
 * @brief definition of Loger class
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#include <mutex>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <iostream>
#include <thread>
#include <queue>
#include <sstream>
#include <semaphore.h>
#include <thread>
#include <unistd.h>
#include <atomic>
#include <sys/time.h>
#include <sys/stat.h>

#ifndef LOGER_H_
#define LOGER_H_


/** @enum message type
 *  @brief Enumerator storing purpose of message
 */

typedef enum messageType
{
	WARN,  /**< warning message type*/
	MSG,	/**< message type representing messages from clients*/
	DEBUG, /**< message type for debugging data*/
	TRACE, /**< message type tracing application*/
	INFO, /**< information messages*/
	FATAL, /**< fatal errors of server*/
	ERR  /**< handled errors of server*/
} tmessageType;

/** @struct logMsg
 *  @brief Stores log message data
 */

typedef struct logMsg
{
	std::string _msg;  /**<text of message*/
	std::string _MsgType;  /**< text representation of type of message*/
	struct timeval _TimeStamp;  /**< time when occured*/
	std::thread::id _thrId;  /**< thread ID*/
	/**Method parse configuration file
	* @param message text of message
	* @param MsgType text representation of type
	* @param timestamp time when event happend
	* @param thrID ide of thread which loged this event
	*/
	logMsg(std::string Msg,std::string MsgType,struct timeval timestamp,std::thread::id thrID);  
} tlogMsg;

/** @class Loger
 *  @brief Class responsible for loging of messages
 */

class Loger
{
	private:
		int _verbosity;  /**< level of logging*/
		std::mutex *_WriteSemaphore; /**< member for mutual exclusion parts*/
		int _FileSize; /**< actual file size*/
		int _FilesCount;  /**< actual files count*/
		int _MaxFileSize;  /**< maximal file size*/
		int _MaxFilesCount; /**< maximal file size*/
		int _ActualFile;  /**< number of file which is currently used for logging*/
		bool _toSTD;
		std::string _FileName; /**< File naming*/
		std::ofstream _Log; /**< opened file stream*/
		/**Method to open files
		 */
		void OpenFile();
		/**Method parse configuration file
		 * @param std::string representation of level
		 * @param std::string with message
		 */
		void QueueMsg(std::string MT,std::string MSG);
		std::string _appName;  /**< application name*/
		sem_t _msgCounter;  /**< semaphore counting messages which are waiting to be logged*/
		std::queue<tlogMsg*> *_msgQueue; /**< queue of messages*/
		/**Method called in separate thread used for logging logic
		 */
		void Dequeue();
		std::thread _worker; /**< thread used to log messages*/
		std::atomic<bool> _teminate; /**< termination condition*/
		std::string _path; /**< folder path to log in*/
	public:
		/**Constructor of class
		 */
		Loger();
		/**Destructor of class
				 */
		~Loger();
		/**Method to set termination flag for separate thread
				 */
		void SetTerminate(){this->_teminate = true;};
		/**Method called by other threads to log  message
		 * @param MT enumeration representation of message type
		 * @param message std::string representation of text from message
				 */
		void WriteMessage (tmessageType MT,std::string message);
		/**Method to set up logger object
		 * @param Verbosity int level of logging
		 * @param FilesCount int representation of max files count
		 * @param LinesCount int representation of max file size int MB
		 * @param FileName std::string representing name of files
		 * @param Path std::string representing Path to folder where are logs stored
		 * @param AppName std::string representing application name
				 */
		void SetLogger(int Verbosity, int FilesCount, int LinesCount, std::string FileName, std::string Path ,std::string AppName, bool ToSTD);

};


#endif /* LOGER_H_ */
