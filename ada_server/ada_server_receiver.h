/**
 * @file server.h
 * 
 * @brief JImplementacia servera - hlavickovy subor
 *
 * @author xblaho03 
 * @version 1.0
 */

#include<string>  //c++ znakove retazce
#include<cstring>
#include <arpa/inet.h> //kniznica pre internetove operacie
#include <ctime>  // kniznica nutna pre casove razitka
//kniznice na pracu s DB
#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include <mutex>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sys/poll.h>
#include <chrono>  //kniznica pre meranie casu
#include <iostream> //kniznica pre vystup na teminal
#include <netdb.h> //kniznica pre sietove operacie
#include <cstdlib>  //kniznica pre standarnde funkcie a dynamicku pamat
#include <netinet/in.h>  //kniznica pre sietovy protokol
#include <sys/socket.h>  //kniznica pre sockety
#include <sys/types.h>  //kniznica pre systemove typy
#include <string.h>  //kniznica pre znakove retazce
#include <unistd.h>  //kniznica pre standardne symbolicke konstanty a typy
#include <signal.h>  //kniznica pre signaly
#include <sys/wait.h> //kniznica pre funkciu wait
#include <atomic>
#include <exception> //kniznica pre bok try/catch
#include <thread> 
#include <semaphore.h>
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "loger.h"
#include "config.h"
#include "databaseConnectionContainer.h"
#include "DBHandler.h"
#include "structures.h"
#include "notif/Notificator.h"
#include "notif/LimitExceededNotification.h"
#include "notif/Notificator.h"


/** @brief Class MessageParser

    Abstraktna Trieda MessageParser 
    */

class MessageParser
{
	private:
		const std::string _Name="MessageParser";
	protected:
		tmessage* _message;			/**< spracovana sprava*/
		int _AnswerSize;			/**< velkost odpovede*/
		pugi::xml_node *_adapter,_device;
		

	public:
		MessageParser() {};
		virtual bool parseMessage(pugi::xml_node *adapter,float FM,float CP) = 0;
		virtual std::string CreateAnswer(int value) = 0;
		void setAdapterIP(in_addr IP){this->_message->adapter_ip=IP;};
		tmessage* ReturnMessage();
		int AnswerSize() {return (this->_AnswerSize);};
		virtual  ~MessageParser() { };
		virtual void GetTimeStamp()=0;
		virtual void GetID()=0;
		virtual void GetFM()=0;
		virtual void GetCP()=0;
		virtual void GetDeviceID()=0;
		virtual void GetBattery()= 0;
		virtual void GetSignal()= 0;
		virtual bool GetValues()= 0;
		virtual void GetValuesCount()= 0;
		virtual void GetState() = 0;
};

/** @brief Class ProtocolV1MessageParser

    Trieda sluziaca na spracovanie spravy od adaptera vo verzii protokolu 1
    */

class ProtocolV1MessageParser :public MessageParser
{
	private:
		const std::string _Name="ProtocolV1MessageParser";
	protected:
		virtual void GetTimeStamp();
		virtual void GetID();
		virtual void GetFM();
		virtual void GetCP();
		virtual void GetDeviceID();
		virtual void GetBattery();
		virtual void GetSignal();
		virtual bool GetValues();
		virtual void GetValuesCount();
		virtual void GetState();
	public:
		ProtocolV1MessageParser() ;
		virtual bool parseMessage(pugi::xml_node *adapter,float FM,float CP);
		virtual std::string CreateAnswer(int value);
		~ProtocolV1MessageParser();
};

/** @brief Class DBHandler

    Trieda sluziaca na pracu s databazou
    */



/** @brief Class ConnectionHandler

    Trieda sluziaca na obsluhu spojeni, vytvaranie procesov a prijimanie/odosielanie sprav
    */

class ConnectionHandler 
{
	private:
		const std::string _Name="ConnectionHandler";
		int _port;		/**< port na ktorom bude server pocuvat*/
		int s;			/**< socket servra*/
	public:
		ConnectionHandler();
		int Listen();
		int ReciveConnection();	
		~ConnectionHandler();
};

class ConnectionServer
{
	private:
		const std::string _Name="ConnectionServer";
		std::chrono::system_clock::time_point start,end;
		int com_s;
		DBHandler *database;    /**< ukazatel na objekt DBHandler*/
		MessageParser *MP;	/**< ukazatel na objekt MessageParser*/
		tmessage *parsedMessage; /**< spracovana sprava*/
		std::string response;		/**< ukazatel na spravu odpovede*/
		int GetData();
		void StoreData();
		void HandleConnection(in_addr IP);
		void Notify();
	public:
		ConnectionServer(int s);
		void HandleConnectionCover(in_addr IP);
		~ConnectionServer();
};

/** @brief Class OutputWriter

    Trieda sluziaca na vypis hlaseni servra 
    */

