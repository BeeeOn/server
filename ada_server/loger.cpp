#include "loger.h"

	/**********************************************Start of OutputWriter section******************************************************************/

/** Metoda pre vypis spravy servra na terminal 
 * @param MT - typ spravy
 * @param message - obsah spravy
 */

Loger::Loger()
{
	this->_WriteSemaphore = new std::mutex();
	sem_init(&(this->_msgCounter),0,0);
	this->_msgQueue = new std::queue<tlogMsg*>;
	this->_teminate = false;
}

Loger::~Loger()
{
	this->SetTerminate();
	while (!this->_msgQueue->empty())
	{
		sleep(1);
	}
	//delete (this->_worker);
	delete (this->_WriteSemaphore);
	delete (this->_msgQueue);
	std::cout<<this->_FileSize<<"\n";
	sem_destroy(&(this->_msgCounter));
}

void Loger::QueueMsg(std::string MT, std::string MSG)
{
	this->_WriteSemaphore->lock();
	this->_msgQueue->push(new tlogMsg(MSG,MT,std::time(nullptr),std::this_thread::get_id()));
	sem_post(&(this->_msgCounter));
	this->_WriteSemaphore->unlock();
};

void Loger::WriteMessage (tmessageType MT,std::string message)
{

	switch(MT)
	{
		case FATAL:
		/*	*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") FATAL ERROR : "<<message<<std::endl;
			outp->flush();*/
			this->QueueMsg("FATAL",message);
			break;
		case ERR:
			if (_verbosity>0)  //ak je zapnuty len vypis chyb servra 
			{
		/*		*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") ERROR : "<<message<<std::endl;
				outp->flush();*/
				this->QueueMsg("ERROR",message);
			}
			break;
		case INFO:
			if (_verbosity>1)  //ak je zapnuty len vypis chyb servra 
			{
				this->QueueMsg("INFO ",message);
			/*	*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") INFO : "<<message<<std::endl;
				std::cout.flush();*/
			}
			break;
		case WARN:
			if (_verbosity>2)  //ak je zapnuty len vypis chyb servra
			{
				this->QueueMsg("WARN ",message);
			/*	*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") WARN : "<<message<<std::endl;
				std::cout.flush();*/
			}
			break;
		case MSG:
			if (_verbosity>3)  //ak je zapnuty len vypis chyb servra
			{
				this->QueueMsg(" MSG ",message);
			/*	*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") MSG : "<<message<<std::endl;
				outp->flush();*/
			}
			break;
		case TRACE:
			if (_verbosity>4)  //ak je zapnuty len vypis chyb servra
			{
				this->QueueMsg("TRACE",message);
			/*	*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") TRACE : "<<message<<std::endl;
				outp->flush();*/
			}
			break;
		default:
			break;
	}
}

/** Metoda pre nastavenie urovne vypisovania dat 
 * @param Verbosity - uroven vypisovania dat*/

void Loger::SetLogger(int Verbosity, int FilesCount, int LinesCount, std::string FileName, std::string AppName)
{
	this->_verbosity = Verbosity;
	std::cout.precision(10);
	this->_MaxFilesCount=FilesCount;
	this->_MaxFileSize=LinesCount*1000000;
	this->_FileSize = 0;
	this->_FileName = FileName;
	this->_FilesCount = 0;
	this->_appName = AppName;
	if (_MaxFilesCount<=0)
	{
		_MaxFilesCount = 1;
	}
	if (_MaxFileSize<=0)
	{
		_MaxFileSize = 1000;
	}
	if (_FileName.compare("")==0)
	{
		_FileName="ada_server";
	}
	this->OpenFile();
	this->_worker = std::thread(&Loger::Dequeue,this);
	this->_worker.detach();
}

void Loger::OpenFile()
{
	this->_Log.open(this->_FileName + "." + std::to_string(this->_FilesCount) + ".log", std::ios::out|std::ios::trunc);
}

logMsg::logMsg(std::string Msg,std::string MsgType,std::time_t timestamp,std::thread::id thrID)
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
		tmp = localtime(&(message->_TimeStamp));
		strftime(timebuf,sizeof(timebuf),"%y/%m/%d_%H:%M:%S",tmp);
		std::stringstream ss;
		ss << message->_thrId;
		std::string MSGstr = this->_appName + " [" + ss.str() + "] (" + timebuf + ") " + message->_MsgType + " : " + message->_msg + "\n";
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
				std::cerr<<"Server Receiver ERROR : Unable to open Log file logging to screen!"<<std::endl;
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

	/**********************************************End of OutputWriter section******************************************************/

