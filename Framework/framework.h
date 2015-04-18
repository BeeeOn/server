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

//Dalsi includy
#include "../pugixml.hpp"
#include "../pugiconfig.hpp"
#include "../loger.h"
#include "../config.h"
#include "structures.h"
#include "../notif/Notificator.h"
#include "../notif/LimitExceededNotification.h"
#include "../notif/Notificator.h"
#include "../databaseConnectionContainer.h"
#include "DBFWHandler.h"
#include "config.h"


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

class MessageParser
{
private:
	const std::string _Name = "MessageParser";
protected:
	tmessage* _message;			/**< spracovana sprava*/
	int _AnswerSize;			/**< velkost odpovede*/
	pugi::xml_node *_adapter, _device;


public:
	MessageParser() {};
	virtual bool parseMessage(pugi::xml_node *adapter, float FM, float CP) = 0;
	virtual std::string CreateAnswer(int value) = 0;
	void setAdapterIP(in_addr IP){ this->_message->adapter_ip = IP; };
	tmessage* ReturnMessage();
	int AnswerSize() { return (this->_AnswerSize); };
	virtual  ~MessageParser() { };
	virtual void GetTimeStamp() = 0;
	virtual void GetID() = 0;
	virtual void GetFM() = 0;
	virtual void GetCP() = 0;
	virtual void GetDeviceID() = 0;
	virtual void GetBattery() = 0;
	virtual void GetSignal() = 0;
	virtual bool GetValues() = 0;
	virtual void GetValuesCount() = 0;
	virtual void GetState() = 0;
};

/** @brief Class ProtocolV1MessageParser

Trieda sluziaca na spracovanie spravy od adaptera vo verzii protokolu 1
*/

class ProtocolV1MessageParser :public MessageParser
{
private:
	const std::string _Name = "ProtocolV1MessageParser";
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
	ProtocolV1MessageParser();
	virtual bool parseMessage(pugi::xml_node *adapter, float FM, float CP);
	virtual std::string CreateAnswer(int value);
	~ProtocolV1MessageParser();
};




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
		int port;			// Port klienta se kterým se komunikuje
		MessageParser *MP;					/**< ukazatel na objekt MessageParser*/
		tmessage *parsedMessage;			/**< spracovana sprava*/
		DBHandler *database;
	public:
		FrameworkServerHandle(int init_socket, int init_port);
		void HandleClientConnection();
		//Handlers
		void HandleAdapterMessage(std::string data, Loger *Log, FrameworkConfig *FConfig);
		void HandleUIServerMessage(std::string data, Loger *Log, FrameworkConfig *FConfig);
		void HandleAlgorithmMessage(std::string data, Loger *Log, FrameworkConfig *FConfig);
		int spawn(char* program, char** arg_list);
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


