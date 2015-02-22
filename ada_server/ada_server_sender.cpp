#define DEF_PORT 7978

#define SWAP(X,Y) { \
       char temp = X ; \
       X = Y ; \
       Y = temp ; \
    }

#include "ada_server_sender.h"

using namespace soci;
using namespace pugi;

Loger *Log;
Config *c;
Listener *L;
DatabaseConnectionContainer *cont = NULL;
sem_t connectionSem;
std::atomic<unsigned long> connCount;

void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		Log->WriteMessage(ERR,"SIGINT received stopping server!");
		delete (L);
		delete (c);
		delete (cont);
		sem_destroy(&connectionSem);
		delete(Log);
	}
	exit(EXIT_SUCCESS);
}

int Listener::Listen ()  //funkcia na vytvorenie spojenia a komunikaciu s klientom
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Listen");
	struct sockaddr_in sin;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)  //vytvorime socket
	{
	  Log->WriteMessage(FATAL,"Creating socket failed");  //ak nastala chyba
	  Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return 1;
	}
	sin.sin_family = PF_INET;   //nastavime komunikaciu na internet
	sin.sin_port = htons (_port);  //nastavime port
	sin.sin_addr.s_addr = INADDR_ANY;  //nastavime IP adresu
	if (bind(s,(struct sockaddr *)&sin , sizeof(sin)) < 0)  //pripojime socket na port
	{
	  Log->WriteMessage(FATAL,"Error while binding socket");  //ak je obsadeny alebo nieco ine sa nepodari vratime chybu
	  Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return 1;
	}
	else
	{
		Log->WriteMessage(MSG,"Socket successfully binded");
	}
	if ((listen (s,10))<0) //chceme na sockete pocuvat
	{
	  Log->WriteMessage(FATAL,"Unable to listen");
	  Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return 1;
	}
	else
	{
		Log->WriteMessage(MSG,"Listenin on port :" + std::to_string(_port));
	}
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	return 0;
}


/** Metoda pre prijatie pripojenia na sockete servra
    */
int Listener::ReciveConnection()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::ReciveConnection");
	struct sockaddr_in sin;
	int com_s;  //nastavime si este jeden socket
	socklen_t s_size;
	s_size=sizeof(sin);
	while (1)
	{
		if ((com_s=accept(s,(struct sockaddr *)&sin ,&s_size )) < 0)  //budeme na nom prijimat data
		{
			Log->WriteMessage(FATAL,"Unable to accept");
			Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
			return 1;
		}
		sem_wait(&connectionSem);
		connCount++;
		Log->WriteMessage(INFO,"CONNECTION COUNT : " + std::to_string(connCount));
		try
		{
			ConnectionServer *CS = new ConnectionServer(com_s);
			std::thread worker(&ConnectionServer::HandleRequestCover,CS);
			worker.detach();
		}
		catch (std::bad_alloc &e)
		{
			std::string ErrMessage = "Allocation of ConnectionServer Failed : ";
			ErrMessage.append(e.what());
			Log->WriteMessage(ERR, ErrMessage + "Client won't be served!");
			close (com_s);
		}
		catch(std::exception &e)
		{
			Log->WriteMessage(ERR, "Client won't be served unable to crate thread!");
			close (com_s);
		}

	//close (com_s);  //zavreme socket
	}
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
	return 0;  //vratime ze je vsetko v poriadku
}

/** Konstruktor objektu vytvoreneho z triedy Listener
    */

Listener::Listener()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Listener");
	_port = c->Port();
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listener");
}

/** Destruktor objektu vytvoreneho z triedy Listener */

Listener::~Listener()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::~Listener");
	close(this->s);
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::~Listener");
}

bool Sender::Connect (std::string Message,std::string IP)  //pripojenie na server a komunikacia s nim
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Connect");
	sockaddr_in SvSoc;
	int s,Err;
	s = socket(AF_INET, SOCK_STREAM, 0);//vytvorenie socketu
	if(s < 0)
	{
		Log->WriteMessage(ERR,"Creating of socket failed ");
		return  false; //chyba pri vytvarani socketu
	}
	std::string opt;
	opt = "tap0";
	if (setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, opt.c_str(), 4) < 0)
	{
		Log->WriteMessage(ERR,"Unable to switch socket to VPN tap0");
	}
	in_addr adapterIP;
	if (inet_pton(AF_INET, IP.c_str(), &adapterIP)!=1)
	{
		Log->WriteMessage(ERR,"Unable to convert IP address");
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
		return false;
	}
	SvSoc.sin_port = htons(DEF_PORT);
	SvSoc.sin_family = AF_INET;  //nastavenie socketu servra
	SvSoc.sin_addr.s_addr = adapterIP.s_addr;
	//memcpy(&SvSoc.sin_addr.s_addr ,Adapter->h_addr, Adapter->h_length);
	if(connect(s,(sockaddr *) &SvSoc, sizeof(SvSoc)) < 0)//pripojenie na server
	{
		Log->WriteMessage(ERR,"Unable to connect to : " + IP);
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
		return false;  //nepodarilo sa pripojit na server
	}
	if ((Err=send(s, Message.c_str(), Message.size(), 0))<0)  //odoslanie poziadavky na server
	{
		Log->WriteMessage(ERR,"Enable to send message to : " + IP);
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
		return false;
	}
	close (s);
	return true;
}

bool ConnectionServer::HandleRequest ()  //TODO: spravit bool return type
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
	char buffer[1024];  //natavime buffer
	ssize_t DataSize=0;
	std::string data;
	data.clear();
	while (1)
	{
		if ((DataSize = read(this->com_s, &buffer, 1024)) < 0) //prijmeme dlzku prichadzajucich dat (2 byty kratkeho integera bez znamienka)
		{
			Log->WriteMessage(ERR,"Reading data from client failed");
			Log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
			return false;
		}
		if (DataSize==0)  //ak nic neprislo ukoncime proces
		{
			return false;
		}
		data.append(buffer,DataSize);
		if (data.find("</request>")!=std::string::npos) //TODO : /> + poll
		{
			break;
		}
	}
	Log->WriteMessage(INFO,"Data: " + data);
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());
	if (result.status!=status_ok)
	{
		std::string Err = result.description();
		Log->WriteMessage(WARN,"Message XML error :" + Err);
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		return false;
	}
	xml_node request = doc.child("request");
	std::string reqType = request.attribute("type").as_string();
	xml_node subject = request.first_child();
	std::string AdapterIP,message;
	Log->WriteMessage(INFO,"Request type :" + reqType);
	if(reqType.compare("delete")==0)
	{
		std::string ID = subject.attribute("id").as_string();
		long long AdapterId = subject.attribute("onAdapter").as_llong();
		Log->WriteMessage(INFO,"Adapter ID :" + std::to_string(AdapterId));
		Log->WriteMessage(INFO,"Device ID :" + ID);
		message = MC->CreateDeleteMessage(ID);
		database->GetAdapterData(&AdapterIP,AdapterId);
	}
	else
	{
		if (reqType.compare("switch")==0)
		{
			std::string ID = subject.attribute("id").as_string();
			tconcatenate temp;
			temp.result=subject.attribute("type").as_int();
			long long AdapterId = subject.attribute("onAdapter").as_llong();
			std::string value = subject.first_child().text().as_string();
			Log->WriteMessage(INFO,"Adapter ID :" + std::to_string(AdapterId));
			Log->WriteMessage(INFO,"Device ID :" + ID);
			Log->WriteMessage(INFO,"Device type :" + std::to_string(temp.input[0]));
			Log->WriteMessage(INFO,"Device offset :" + std::to_string(temp.input[1]));
			Log->WriteMessage(INFO,"Device value :" + value);
			std::string stringType;
			std::ostringstream os;
			os << std::hex << temp.input[0];
			stringType = os.str();
			Log->WriteMessage(INFO,"Device hexa type :" + stringType);
			message = this->MC->CreateSwitchMessage(std::to_string(AdapterId),ID,value,stringType,std::to_string(temp.input[1]));
			database->GetAdapterData(&AdapterIP,AdapterId);
		}
		else
		{
			long long int ID = subject.attribute("id").as_llong();
			Log->WriteMessage(INFO,"Adapter ID :" + std::to_string(ID));
			message = MC->CreateListenMessage(std::to_string(ID));
			database->GetAdapterData(&AdapterIP,ID);
		}
	}
	Log->WriteMessage(INFO,"Adapter IP :" + AdapterIP);
	Log->WriteMessage(INFO,"Message: \n" + message);
	bool res = this->s->Connect(message,AdapterIP);
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
	return res;
}

void ConnectionServer::HandleRequestCover()
{
	Log->WriteMessage(TRACE,"Process created!");
	session *Conn = NULL;
	while (Conn==NULL)
	{
		Conn=cont->GetConnection();
		if (Conn==NULL)
		{
			Log->WriteMessage(INFO,"ALL CONNECTIONS ARE USED!");
		}
	}
	try
	{
		this->database = new DBHandler(Conn, Log);
	}
	catch(std::exception &e)
	{
		Log->WriteMessage(ERR, "Client won't be served unable to create DBHandler!");
		close (com_s);
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnectionCover");
		delete (this);
		return;
	}
	std::string Message = "<reply>true</reply>";
	if (!this->HandleRequest())
	{
		 Message = "<reply>false</reply>";
	}
	if ((send(com_s, Message.c_str(), Message.size(), 0))<0)  //odoslanie poziadavky na server
	{
		close (com_s);  //zavreme socket
		Log->WriteMessage(WARN,"Unable to send message to server");
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
	}
	cont->ReturnConnection(database->ReturnConnection());
	sem_post(&connectionSem);
	delete(this);
}

ConnectionServer::ConnectionServer(int Com_s)
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::ConnectionServer");
	this->database = NULL;
	this->com_s = Com_s;
	MC = new MessageCreator();
	s = new Sender();
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ConnectionServer");
}

ConnectionServer::~ConnectionServer()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::~ConnectionServer");
	close(this->com_s);
	delete this->database;
	delete this->MC;
	delete this->s;
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::~ConnectionServer");
}



std::string MessageCreator::CreateDeleteMessage(std::string deviceID)
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateDeleteMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	//server_adapter->set_name("server_adapter");
	server_adapter.append_attribute("protocol_version");
	server_adapter.append_attribute("state");
	server_adapter.append_attribute("id");
	server_adapter.attribute("protocol_version") = "0.1";
	server_adapter.attribute("state") = "clear";
	struct sockaddr_in antelope;
	inet_aton(deviceID.c_str(), &antelope.sin_addr);
	in_addr_t DeviceIP = antelope.sin_addr.s_addr;
	unsigned int DeviceIPint = ntohl (DeviceIP);
	server_adapter.attribute("id") = std::to_string(DeviceIPint).c_str();
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateDeleteMessage");
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result;

};

std::string MessageCreator::CreateListenMessage(std::string AdapterID)
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateListenMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	//server_adapter->set_name("server_adapter");
	server_adapter.append_attribute("protocol_version");
	server_adapter.append_attribute("state");
	server_adapter.append_attribute("adapter_id");
	server_adapter.attribute("protocol_version") = "0.1";
	server_adapter.attribute("state") = "listen";
	server_adapter.attribute("adapter_id") = AdapterID.c_str();
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateListenMessage");
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result;

};

std::string MessageCreator::CreateSwitchMessage(std::string Adapter, std::string deviceId, std::string value, std::string type, std::string offset)
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateSwitchMessage");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	//server_adapter->set_name("server_adapter");
	struct sockaddr_in antelope;
	inet_aton(deviceId.c_str(), &antelope.sin_addr);
	in_addr_t DeviceIP = antelope.sin_addr.s_addr;
	unsigned int DeviceIPint = ntohl (DeviceIP);
	server_adapter.append_attribute("protocol_version") = "0.1";
	server_adapter.append_attribute("state") = "set";
	server_adapter.append_attribute("id") = std::to_string(DeviceIPint).c_str();
	server_adapter.append_attribute("debug_adapter_id") = Adapter.c_str();
	//server_adapter.attribute("protocol_version") = "0.1";
	//server_adapter.attribute("state")
	xml_node value_node = server_adapter.append_child("value");
	value_node.text().set(value.c_str());
	value_node.append_attribute("type") = type.c_str();
	value_node.append_attribute("offset") = offset.c_str();
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateSwitchMessage");
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result;
};

MessageCreator::MessageCreator()
{

};

int main()  //hlavne telo programu
{
	Log = new Loger();
	c = new Config();
	if (!c->setConfig("ada_server_sender.config.xml",1))
	{
		return 1;
	}
	else
	{
		Log->SetLogger(c->Verbosity(),c->MaxFiles(),c->MaxLines(),c->FileNaming(),"ADA SERVER SENDER");
	}
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		Log->WriteMessage(ERR,"Unable to catch SIGINT");
	}
	cont = DatabaseConnectionContainer::CreateContainer(Log,c->DBName(),c->ConnLimit());
	int semVal = cont->Limit();
	Log->WriteMessage(FATAL,"Maximal conn count : " + std::to_string(semVal));
	sem_init(&connectionSem,0,semVal);
	L =  new Listener();
	L->Listen();
	L->ReciveConnection();
	delete (c);
	delete (cont);
	delete (L);
	delete (Log);
	return 0;
}
