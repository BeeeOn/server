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
#include "structures.h"
#include "../lib/pugixml.hpp"
#include "../lib/pugiconfig.hpp"

#ifndef ADAPTER_MESSAGE_PARSER_
#define ADAPTER_MESSAGE_PARSER_

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
#endif