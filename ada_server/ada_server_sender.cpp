#define DEF_PORT 7978

#define SWAP(X,Y) { \
       char temp = X ; \
       X = Y ; \
       Y = temp ; \
    }

#include "ada_server_sender.h"

using namespace soci;
using namespace pugi;

Loger Output;
Config c;
Listener *L;

void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		Output.WriteMessage(ERR,"SIGINT received stopping server!");
		delete(L);
	}
	exit(EXIT_SUCCESS);
}

int Listener::Listen ()  //funkcia na vytvorenie spojenia a komunikaciu s klientom
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::Listen");
	struct sockaddr_in sin;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)  //vytvorime socket
	{
	  Output.WriteMessage(FATAL,"Creating socket failed");  //ak nastala chyba
	  Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return 1;
	}
	sin.sin_family = PF_INET;   //nastavime komunikaciu na internet
	sin.sin_port = htons (_port);  //nastavime port
	sin.sin_addr.s_addr = INADDR_ANY;  //nastavime IP adresu
	if (bind(s,(struct sockaddr *)&sin , sizeof(sin)) < 0)  //pripojime socket na port
	{
	  Output.WriteMessage(FATAL,"Error while binding socket");  //ak je obsadeny alebo nieco ine sa nepodari vratime chybu
	  Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return 1;
	}
	else
	{
		Output.WriteMessage(MSG,"Socket successfully binded");
	}
	if ((listen (s,10))<0) //chceme na sockete pocuvat
	{
	  Output.WriteMessage(FATAL,"Unable to listen");
	  Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return 1;
	}
	else
	{
		Output.WriteMessage(MSG,"Listenin on port :" + std::to_string(_port));
	}
	Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	return 0;
}


/** Metoda pre prijatie pripojenia na sockete servra
    */
int Listener::ReciveConnection()
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::ReciveConnection");
	struct sockaddr_in sin;
	int com_s;  //nastavime si este jeden socket
	socklen_t s_size;
	s_size=sizeof(sin);
	DBHandler *DB = new DBHandler();
	ConnectionServer CS(DB);

	while (1)
	{
		if ((com_s=accept(s,(struct sockaddr *)&sin ,&s_size )) < 0)  //budeme na nom prijimat data
		{
			Output.WriteMessage(FATAL,"Unable to accept");
			Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
			return 1;
		}
		CS.HandleConnection(com_s);
		std::string Message = "<reply>true</reply>";
		if ((send(com_s, Message.c_str(), Message.size(), 0))<0)  //odoslanie poziadavky na server
		{
			close (com_s);  //zavreme socket
			Output.WriteMessage(WARN,"Unable to send message to server");
			Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
		}
	close (com_s);  //zavreme socket
	}
	Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
	return 0;  //vratime ze je vsetko v poriadku
}

/** Konstruktor objektu vytvoreneho z triedy Listener
    */

Listener::Listener()
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::Listener");
	_port = c.Port();
	Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::Listener");
}

/** Destruktor objektu vytvoreneho z triedy Listener */

Listener::~Listener()
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::~Listener");
	close(this->s);
	Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::~Listener");
}

int Sender::Connect (std::string Message,std::string IP)  //pripojenie na server a komunikacia s nim
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::Connect");
	sockaddr_in SvSoc;
	int s,Err;
	s = socket(AF_INET, SOCK_STREAM, 0);//vytvorenie socketu
	if(s < 0)
	{
		Output.WriteMessage(ERR,"Creating of socket failed ");
		return  1; //chyba pri vytvarani socketu
	}
	std::string opt;
	opt = "tap0";
	if (setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, opt.c_str(), 4) < 0)
	{
		Output.WriteMessage(ERR,"Enable to switch socket to VPN tap0");
	}
	in_addr adapterIP;
	inet_pton(AF_INET, IP.c_str(), &adapterIP);
	SvSoc.sin_port = htons(DEF_PORT);
	SvSoc.sin_family = AF_INET;  //nastavenie socketu servra
	SvSoc.sin_addr.s_addr = adapterIP.s_addr;
	//memcpy(&SvSoc.sin_addr.s_addr ,Adapter->h_addr, Adapter->h_length);
	if(connect(s,(sockaddr *) &SvSoc, sizeof(SvSoc)) < 0)//pripojenie na server
	{
		Output.WriteMessage(ERR,"Enable to connect to : " + IP);
		Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
		return 1;  //nepodarilo sa pripojit na server
	}
	if ((Err=send(s, Message.c_str(), Message.size(), 0))<0)  //odoslanie poziadavky na server
	{
		Output.WriteMessage(ERR,"Enable to send message to : " + IP);
		Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::Connect");
	}
	close (s);
	return 0;
}

void ConnectionServer::HandleConnection (int socket)  //TODO: spravit bool return type
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
	char buffer[1024];  //natavime buffer
	ssize_t DataSize=0;
	std::string data;
	data.clear();
	while (1)
	{
		if ((DataSize = read(socket, &buffer, 1024)) < 0) //prijmeme dlzku prichadzajucich dat (2 byty kratkeho integera bez znamienka)
		{
			Output.WriteMessage(ERR,"Reading data from client failed");
			Output.WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
			return;
		}
		if (DataSize==0)  //ak nic neprislo ukoncime proces
		{
			return;
		}
		data.append(buffer,DataSize);
		if (data.find("</request>")!=std::string::npos) //TODO : /> + poll
		{
			break;
		}
	}
	Output.WriteMessage(INFO,"Data: " + data);
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());
	if (result.status!=status_ok)
	{
		std::string Err = result.description();
		Output.WriteMessage(WARN,"Message XML error :" + Err);
		Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		return;
	}
	xml_node request = doc.child("request");
	std::string reqType = request.attribute("type").as_string();
	xml_node subject = request.first_child();
	std::string AdapterIP,message;
	Output.WriteMessage(INFO,"Device ID :" + reqType);
	if(reqType.compare("delete")==0)
	{
		std::string ID = subject.attribute("id").as_string();
		long long AdapterId = subject.attribute("onAdapter").as_llong();
		Output.WriteMessage(INFO,"Adapter ID :" + std::to_string(AdapterId));
		Output.WriteMessage(INFO,"Device ID :" + ID);
		message = MC->CreateDeleteMessage(ID);
		database->GetAdapterData(&AdapterIP,AdapterId);
	}
	else
	{
		long long int ID = subject.attribute("id").as_llong();
		Output.WriteMessage(INFO,"Adapter ID :" + std::to_string(ID));
		message = MC->CreateListenMessage(std::to_string(ID));
		database->GetAdapterData(&AdapterIP,ID);
	}
	Output.WriteMessage(INFO,"Adapter IP :" + AdapterIP);
	Output.WriteMessage(INFO,"Message: \n" + message);
	this->s->Connect(message,AdapterIP);
	Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
	return;
}

ConnectionServer::ConnectionServer(DBHandler *d)
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::ConnectionServer");
	this->database = d;
	MC = new MessageCreator();
	s = new Sender();
	Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::ConnectionServer");
}

ConnectionServer::~ConnectionServer()
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::~ConnectionServer");
	close(this->com_s);
	delete this->database;
	delete this->MC;
	Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::~ConnectionServer");
}


DBHandler::DBHandler()
{
	sql = new session(postgresql, "dbname=" + c.DBName());

};

void DBHandler::GetAdapterData(std::string *adapterIP, long int ID)
{
	try
	{
		*sql<<"SELECT ip_address FROM adapters where adapter_id=" + std::to_string(ID) + ";" , into(*adapterIP);
	}
	catch(std::exception const &e)
	{
		std::string ErrorMessage = "Database Error : ";
		ErrorMessage.append (e.what());
		Output.WriteMessage(ERR,ErrorMessage );
	}
}

std::string MessageCreator::CreateDeleteMessage(std::string deviceID)
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::CreateDeleteMessage");
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
	Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateDeleteMessage");
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result;

};

std::string MessageCreator::CreateListenMessage(std::string AdapterID)
{
	Output.WriteMessage(TRACE,"Entering " + this->_Name + "::CreateListenMessage");
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
	Output.WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateListenMessage");
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + writer.result;

};

MessageCreator::MessageCreator()
{

};

int main()  //hlavne telo programu
{
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		Output.WriteMessage(ERR,"Unable to catch SIGINT");
	}
	if (!c.setConfig("ada_server_sender.config.xml",1))
	{
		return 1;
	}
	else
	{
		Output.SetLogger(c.Verbosity(),c.MaxFiles(),c.MaxLines(),c.FileNaming(),"ADA SERVER SENDER");
	}
	L =  new Listener();
	L->Listen();
	L->ReciveConnection();

	return 0;
}
