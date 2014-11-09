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
#include<iostream>
#include <thread>

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

class Loger
{
	private:
		int _verbosity;  /**< miera vypisovania hlaseni servera*/
		std::mutex *_WriteSemaphore;
		int _LinesCount;
		int _FilesCount;
		int _MaxLinesCount;
		int _MaxFilesCount;
		int _ActualFile;
		std::string _FileName;
		std::ofstream _Log;
		void OpenFile();
		std::string _appName;

	public:
		Loger();
		~Loger();
		void WriteMessage (tmessageType MT,std::string message);
		void SetLogger(int Verbosity, int FilesCount, int LinesCount, std::string FileName, std::string AppName);

};


#endif /* LOGER_H_ */
