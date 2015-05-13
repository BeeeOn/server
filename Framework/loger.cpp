/**
* @file loger.cpp
*
* @Implementace Metod pro logovani vypisu do souboru
*
*/

#include "loger.h"


Loger::Loger()
{
	this->_WriteSemaphore = new std::mutex();
	sem_init(&(this->_msgCounter),0,0);
	this->_msgQueue = new std::queue<tlogMsg*>;
	this->_teminate = false;
	this->_ActualFile = 0;
	this->_MaxFileSize = 0;
	this->_FileSize =0;
	this->_verbosity = 0;
	this->_MaxFilesCount = 0;
	this->_FilesCount = 0;
}

Loger::~Loger()
{
	this->SetTerminate();
	this->QueueMsg("LOGINFO","***********Closing log***********");
	this->_worker.join();
	delete (this->_WriteSemaphore);
	delete (this->_msgQueue);
	std::cout<<this->_FileSize<<"\n";
	sem_destroy(&(this->_msgCounter));
}

/** Metoda pro vypis zpravy serveru na terminal
* @param MT - typ spravy
* @param message - obsah zpravy
*/
void Loger::WriteMessage(tmessageType MT, std::string message)
{

	switch (MT)
	{
	case FATAL:
		this->QueueMsg("FATAL", message);
		break;
	case ERR:
		if (_verbosity>0)
		{
			this->QueueMsg("ERROR", message);
		}
		break;
	case INFO:
		if (_verbosity>1)
		{
			this->QueueMsg("INFO ", message);
		}
		break;
	case WARN:
		if (_verbosity>2)
		{
			this->QueueMsg("WARN ", message);
		}
		break;
	case MSG:
		if (_verbosity>3)
		{
			this->QueueMsg(" MSG ", message);
		}
		break;
	case TRACE:
		if (_verbosity>4)
		{
			this->QueueMsg("TRACE", message);
		}
		break;
	default:
		break;
	}
}

/** Metoda pro nastaveni Logovani
* @param Verbosity - uroven vypisovani dat
* @param FilesCount - maximalni pocet souboru k vytvoreni
* @param LinesCount - maximalni pocet zaznamu/radku na soubor
* @param FileName - prefix nazvu logovaciho souboru
* @param Path - cesta, kam se budou logovaci soubory ukladat
* @param AppName - nazev aplikace, ktera se bude uvadet v logovani
*/
void Loger::SetLogger(int Verbosity, int FilesCount, int LinesCount, std::string FileName, std::string Path, std::string AppName)
{
	this->_verbosity = Verbosity;
	std::cout.precision(10);
	this->_MaxFilesCount = FilesCount;
	this->_MaxFileSize = LinesCount * 1000000;
	this->_FileSize = 0;
	this->_FileName = FileName;
	this->_FilesCount = 0;
	this->_appName = AppName;
	this->_path = Path;
	if (_MaxFilesCount <= 0)
	{
		_MaxFilesCount = 1;
	}
	if (_MaxFileSize <= 0)
	{
		_MaxFileSize = 1000;
	}
	if (_FileName.compare("") == 0)
	{
		_FileName = "framework";
	}
	this->OpenFile();
	this->_worker = std::thread(&Loger::Dequeue, this);
}

void Loger::QueueMsg(std::string MT, std::string MSG)
{
	this->_WriteSemaphore->lock();
	struct timeval str;
	gettimeofday(&str,NULL);
	this->_msgQueue->push(new tlogMsg(MSG,MT,str,std::this_thread::get_id()));
	sem_post(&(this->_msgCounter));
	this->_WriteSemaphore->unlock();
};



void Loger::OpenFile()
{
	int result;
	result = mkdir(this->_path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (result==0||errno==EEXIST)
	{
		this->_Log.open(this->_path + "/" + this->_FileName + "." + std::to_string(this->_FilesCount) + ".log", std::ios::out|std::ios::trunc);
	}
	}

logMsg::logMsg(std::string Msg,std::string MsgType,struct timeval timestamp,std::thread::id thrID)
{
	this->_msg = Msg;
	this->_thrId = thrID;
	this->_TimeStamp = timestamp;
	this->_MsgType = MsgType;
};

void Loger::Dequeue()
{
	tlogMsg *message;
	while((!this->_teminate)||(!this->_msgQueue->empty()))
	{
		sem_wait(&(this->_msgCounter));
		this->_WriteSemaphore->lock();
		message = this->_msgQueue->front();
		this->_msgQueue->pop();
		this->_WriteSemaphore->unlock();
		char timebuf[200];
		struct tm *tmp;
		tmp = localtime(&(message->_TimeStamp.tv_sec));
		strftime(timebuf,sizeof(timebuf),"%y/%m/%d_%H:%M:%S",tmp);
		std::stringstream ss;
		ss << message->_thrId;
		std::string MSGstr = this->_appName + " [" + ss.str() + "] (" + timebuf + "." + std::to_string(message->_TimeStamp.tv_usec) +") " + message->_MsgType + " : " + message->_msg + "\n";
		if ((this->_FileSize+=MSGstr.size())>this->_MaxFileSize)
		{
			this->_FileSize=0;
			this->_Log.close();
			if(++this->_FilesCount>=this->_MaxFilesCount)
			{
				this->_FilesCount=0;
				this->OpenFile();
			}
		}
		std::ostream *outp;
		if (this->_Log.is_open())
		{
			outp = &_Log;
		}
		else
		{
			this->OpenFile();
			if(!this->_Log.is_open())
			{
				std::cerr<<"Framework ERROR : Unable to open Log file logging to screen! Probably problem with privilegues! Try start Framework using sudo."<<std::endl;
				outp = &(std::cout);
			}
			else
			{
				outp = &_Log;
			}
		}
		*outp<<MSGstr;
		outp->flush();
		delete(message);
	}
};