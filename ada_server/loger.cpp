#include "loger.h"

	/**********************************************Start of OutputWriter section******************************************************************/

/** Metoda pre vypis spravy servra na terminal 
 * @param MT - typ spravy
 * @param message - obsah spravy
 */

Loger::Loger()
{
	this->_WriteSemaphore = new std::mutex();
}

Loger::~Loger()
{
	delete (this->_WriteSemaphore);
}

void Loger::WriteMessage (tmessageType MT,std::string message)
{
	this->_WriteSemaphore->lock();
	if (++this->_LinesCount>this->_MaxLinesCount)
	{
		this->_LinesCount=1;
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
	std::time_t result = std::time(nullptr);
	char timebuf[200];
	struct tm *tmp;
	tmp = localtime(&(result));
	strftime(timebuf,sizeof(timebuf),"%y%m%d_%H:%M:%S",tmp);
	switch(MT)
	{
		case FATAL:
			*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") FATAL ERROR : "<<message<<std::endl;
			outp->flush();
			break;
		case ERR:
			if (_verbosity>0)  //ak je zapnuty len vypis chyb servra 
			{
				*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") ERROR : "<<message<<std::endl;
				outp->flush();
			}
			break;
		case INFO:
			if (_verbosity>1)  //ak je zapnuty len vypis chyb servra 
			{
				*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") INFO : "<<message<<std::endl;
				std::cout.flush();
			}
			break;
		case WARN:
			if (_verbosity>2)  //ak je zapnuty len vypis chyb servra
			{
				*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") WARN : "<<message<<std::endl;
				std::cout.flush();
			}
			break;
		case MSG:
			if (_verbosity>3)  //ak je zapnuty len vypis chyb servra
			{
				*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") MSG : "<<message<<std::endl;
				outp->flush();
			}
			break;
		case TRACE:
			if (_verbosity>4)  //ak je zapnuty len vypis chyb servra
			{
				*outp<<this->_appName<<" ["<<std::this_thread::get_id()<<"] ("<<timebuf<<") TRACE : "<<message<<std::endl;
				outp->flush();
			}
			break;
		default:
			break;
	}
	this->_WriteSemaphore->unlock();
}

/** Metoda pre nastavenie urovne vypisovania dat 
 * @param Verbosity - uroven vypisovania dat*/

void Loger::SetLogger(int Verbosity, int FilesCount, int LinesCount, std::string FileName, std::string AppName)
{
	this->_verbosity = Verbosity;
	std::cout.precision(10);
	this->_MaxFilesCount=FilesCount;
	this->_MaxLinesCount=LinesCount;
	this->_LinesCount = 0;
	this->_FileName = FileName;
	this->_FilesCount = 0;
	this->_LinesCount = 0;
	this->_appName = AppName;
	if (_MaxFilesCount<=0)
	{
		_MaxFilesCount = 1;
	}
	if (_MaxLinesCount<=0)
	{
		_MaxLinesCount = 1000;
	}
	if (_FileName.compare("")==0)
	{
		_FileName="ada_server";
	}
	this->OpenFile();
}

void Loger::OpenFile()
{
	this->_Log.open(this->_FileName + "." + std::to_string(this->_FilesCount) + ".log", std::ios::out|std::ios::trunc);
}

	/**********************************************End of OutputWriter section******************************************************/

