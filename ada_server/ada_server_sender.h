#include <string>
#include <iostream>
#include <netdb.h>
#include <errno.h>
#include <cstdlib>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <locale.h>
#include <fstream>
#include <signal.h>
#include <sys/wait.h>
#include <vector>
#include <pwd.h>
#include <thread>
#include <arpa/inet.h>
#include <exception>
#include "pugixml.hpp"
#include "pugiconfig.hpp"
#include <soci.h>
#include <postgresql/soci-postgresql.h>
#include "config.h"
#include "loger.h"


typedef struct xml_string_writer: pugi::xml_writer
{
    std::string result;
    const std::string _Name="xml_string_writer";
    virtual void write(const void* data, size_t size)
    {
        result += std::string(static_cast<const char*>(data), size);
    }
} tstringXMLwriter;


class Sender
{
	private:
    	const std::string _Name="Sender";
		int _port;
		int s;
	public:
		int Connect(std::string Message,std::string IP);
};

class Listener
{
	private:
    	const std::string _Name="Listener";
		int _port;		/**< port na ktorom bude server pocuvat*/
		int s;			/**< socket servra*/
	public:
		Listener();
		int Listen();
		int ReciveConnection();
		~Listener();
};


class DBHandler
{
	private:
		const std::string _Name = "DBHandler";
		soci::session *sql;

	public:
		DBHandler();
		void GetAdapterData(std::string *adapterIP, long int ID);
};

class MessageCreator
{
	private:
		const std::string _Name = "MessageCreator";

	public:
	std::string CreateDeleteMessage(std::string deviceID);
	//std::string CreateSwitchMessage(std::string type, std::string val, std::string actorIP);
	std::string CreateListenMessage(std::string AdapterID);
	MessageCreator();
};

class ConnectionServer
{
	private:
		const std::string _Name = "ConnectionServer";
		//std::chrono::system_clock::time_point start,end;
		int com_s;
		DBHandler *database;    /**< ukazatel na objekt DBHandler*/
		//tmessage *parsedMessage; /**< spracovana sprava*/
		std::string response;		/**< ukazatel na spravu odpovede*/
		int GetData();
		MessageCreator *MC;
		Sender *s;
		//void StoreData();
	public:
		ConnectionServer(DBHandler *d);
		void HandleConnection(int socket);
		~ConnectionServer();
};
