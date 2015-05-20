/**
* @file framework.h
*
* Definice trid pro Modularni prostredi.
*
* @author Martin Rasovsky (xrasov01@stud.fit.vutbr.cz)
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

/** Struktura pro uchovavani informaci ohledne predanych parametru a devices v prubehu parsovani dat ohledne aplikacnich modulech ulozenych v databazi.
*
*/
typedef struct chldMsg
{
	std::string nameOfElement;		/** Nazev elementu (napr pro <par .../> je to "par".  */
	std::string id;					/** Id senzoru ci aktoru, pokud se jedna o element dev. */
	std::string type;				/** Typ senzoru ci aktoru, pokud se jedna o element dev. */
	std::string pos;				/** Pozice predaneho elementu. (at uz "dev", ci "par")*/
	std::string text;				/** Obsah elementu. <par> OBSAH ELEMENTU </par>*/
} tchldMsg;

/** Struktura uchovavajici informace o jednom aplikacnim modulu nactenem z databaze.
*
*/
typedef struct alg
{
	std::string usersAlgorithmsId;	/** Primarni klic zaznamu.*/		
	std::string atype;				/** Typ aplikacniho modulu. Definovany v konfiguracnim souboru a nasledne take ulozeny v tabulce u_algorithms.*/
	std::string enable;				/** Urceni, zda se jedna o zapnuty ci vypnuty palikacni modul.*/
	std::string name;				/** Nazev aplikacniho modulu definovany uzivatelem. (napr. Hlidani teploty)*/
	std::vector<tchldMsg*> childs;	/** Kolekce informaci o definovanych parametrech a zarizenich uzivatelem, ktere jsou prirazeny k aplikacnimu modulu.*/
} talg;

/** Struktura predavajici informace ohledne notifikace.
*
*/
typedef struct notify
{
	unsigned short int notifyType;	/** Typ notifikace (Bude pouzito do budoucna, zatim muzeme vlozit 0 jako obecna notifikace). */
	std::string notifyText;			/** Text zprávy. */
} tnotify;

/** Struktura predavajici informace ohledne zarizeni (aktor, senzor).
*
*/
typedef struct device
{
	unsigned int id;				/** Identifikacni cislo zarizeni. */
	int type;						/** Typ zarizeni. */
	int pos;						/** Pozice zarizeni predana do aplikacniho modulu. */
} tdevice;

/** Struktura predavajici informace o predanem parametru.
*
*/
typedef struct param
{
	int pos;						/** Pozice parametru predaneho do aplikacniho modulu. */
	std::string text;				/** Obsah parametru ve formatu retezce. */
} tparam;

/** Trida slouzici pro pripravu schranek pro naslouchani a tvorbu procesu pro obsluhu projatych spojeni. 
*
*/
class FrameworkServer {
	private:
		sockaddr_in portBind;			/** Nabindovani portu. */				
		int serverSocket;				/** Socket tcp serveru. */
		int port;						/** Port, na kterem server nasloucha. */
	public: 
		FrameworkServer(int init_port);
		void StartServer();
		bool SetUpSockets();
		int AcceptConnection();
		~FrameworkServer();
};

/** Trida slouzici pro obsluhu zprav zasilanych na jednotlive servery. Je navrzena a implementovana tim zpusobem, ze obsluhuje vsechny tri typy serveru dle zadaneho portu.
*
*/
class FrameworkServerHandle {
	private:
		const std::string _Name = "FrameworkServerHandle";
		std::string message;				/** Prijimana zprava. */
		int handledSocket;					/** Obsluhovany socket. */
		int port;							/** Port klienta, se kterym se komunikuje. */
		MessageParser *MP;					/** Ukazatel na objekt MessageParser.*/
		tmessage *parsedMessage;			/** Zpracovana zprava. */
		DBFWHandler *database;				/** Prace s databazi. Je do nej predano sezeni. */
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