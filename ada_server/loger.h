/*
 * loger.h
 *
 *  Created on: Oct 19, 2014
 *      Author: tuso
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

#ifndef LOGER_H_
#define LOGER_H_


/** @enum valueTypes
 *  @brief Enumerator sluziaci na zaznamenanie typu spravy vypisovanej servrom
 */

typedef enum messageType
{
	WARN,
	MSG,
	TRACE,
	INFO,
	FATAL,
	ERR
} tmessageType;

typedef struct logMsg
{
	std::string _msg;
	std::string _MsgType;
	std::time_t _TimeStamp;
	std::thread::id _thrId;
	logMsg(std::string Msg,std::string MsgType,std::time_t timestamp,std::thread::id thrID);
} tlogMsg;

class Loger
{
	private:
		int _verbosity;  /**< miera vypisovania hlaseni servera*/
		std::mutex *_WriteSemaphore;
		int _FileSize;
		int _FilesCount;
		int _MaxFileSize;
		int _MaxFilesCount;
		int _ActualFile;
		std::string _FileName;
		std::ofstream _Log;
		void OpenFile();
		void QueueMsg(std::string MT,std::string MSG);
		std::string _appName;
		sem_t _msgCounter;
		std::queue<tlogMsg*> *_msgQueue;
		void Dequeue();
		std::thread _worker;
		bool _teminate;
	public:
		Loger();
		~Loger();
		void SetTerminate(){this->_teminate = true;};
		void WriteMessage (tmessageType MT,std::string message);
		void SetLogger(int Verbosity, int FilesCount, int LinesCount, std::string FileName, std::string AppName);

};


#endif /* LOGER_H_ */
