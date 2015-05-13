/**
* @file framework.h
*
* @Definice trid pro Modularni prostredi
*
* @author xrasov01
* @version 1.0
*/

#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <ctime>
#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include <mutex>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sys/poll.h>
#include <iostream>
#include <netdb.h>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <atomic>
#include <exception>
#include <thread> 
#include <semaphore.h>
#include <stdlib.h> 
#include <chrono>
#include <sstream>
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include "loger.h"
#include "structures.h"
#include "Notificator/Notificator.h"
#include "Notificator/WatchdogNotif.h"
#include "DBConnectionsContainer.h"
#include "DBFWHandler.h"
#include "config.h"
#include "AdapterMessageParser.h"

#ifndef FRAMEWORK_HEADER_
#define FRAMEWORK_HEADER_

typedef struct chldMsg
{
	std::string nameOfElement;
	std::string id;
	std::string type;
	std::string pos;
	std::string text;
} tchldMsg;

typedef struct alg
{
	std::string usersAlgorithmsId;
	std::string atype;
	std::string enable;
	std::string name;
	std::vector<tchldMsg*> childs;
} talg;

//Struktura pøedávající informace ohlednì notifikace
typedef struct notify
{
	unsigned short int notifyType;	//Typ notifikace (Bude použito do budoucna, zatím mùžeme vložit 0 jako obecná notifikace)
	std::string notifyText;			//Text zprávy
} tnotify;

typedef struct device
{
	int id;
	int type;
	int pos;
} tdevice;

typedef struct param
{
	int pos;
	std::string text;
} tparam;

/** @brief Class FrameworkServer

Tøída sloužící pro pøípravu soketù pro naslouchání a tvorbu procesù pro obsluhu pøijatých spojení.
*/
class FrameworkServer {
	private:
		sockaddr_in portBind;			// Nabindování portu				
		int serverSocket;				// Socket tcp serveru
		int port;
	public: 
		FrameworkServer(int init_port);
		void StartServer();
		bool SetUpSockets();
		int AcceptConnection();
		~FrameworkServer();
};

class FrameworkServerHandle {
	private:
		const std::string _Name = "FrameworkServerHandle";
		std::string message;				// Pøijímaná zpráva
		int handledSocket;					// Obsluhovaný socket
		int port;							// Port klienta se kterým se komunikuje
		MessageParser *MP;					// Ukazatel na objekt MessageParser
		tmessage *parsedMessage;			// Zpracovana zprava
		DBFWHandler *database;
	public:
		FrameworkServerHandle(int init_socket, int init_port);
		void HandleClientConnection();
		//Handlers
		void HandleAdapterMessage(std::string data, Loger *Log, FrameworkConfig *FConfig, DBFWHandler *database, sem_t* dbAccessSem);
		void HandleUIServerMessage(std::string data, Loger *Log, FrameworkConfig *FConfig, DBFWHandler *database, sem_t* dbAccessSem);
		void HandleAlgorithmMessage(std::string data, Loger *Log, FrameworkConfig *FConfig, DBFWHandler *database, sem_t* dbAccessSem);
		int spawn(char* programBinaryName, char** arg_list);
		~FrameworkServerHandle();
		std::string parseParametersToDB(tparam *params, int paramsCnt);
		bool sendMessageToSocket(int socket, string xmlMessage);
		talg * getAlgByUserAlgorithmId(string userAlgId);
		vector<string> parseParametersToVector(string notParsedParams);
		vector<string> explode(string str, char ch);
		vector<talg *> getAllAlgsByAdapterIdAndUserId(string adapterId, string userId);
		char * StringToChar(string toChange);
		bool sendMessageToAdaServer(string xmlMessage);
		//Creating messages for UI server
		std::string createMessageAlgCreated(string algId);
		std::string createMessageFalse(string errcode);
		std::string createMessageAlgs(vector<talg *> allAlgs, string adapterId);
		std::string createMessageTrue();
		string createMessageRequestSwitch(string id, string type, string adapterId);

};
#endif