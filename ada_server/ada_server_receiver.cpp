/**
 * @file server.cpp
 * 
 * @brief Implementacia servera - zdrojovy subor
 *
 * @author xblaho03 
 * @version 1.0
 */
#define DEF_PORT 9091  //defaultny port
//makro pre prehodenie hodnot dvoch premennych
#define SWAP(X,Y) { \
       char temp = X ; \
       X = Y ; \
       Y = temp ; \
    }

#include "ada_server_receiver.h" //hlavickovy subor

using namespace soci;
using namespace pugi;

sem_t connectionSem;
Loger *Log;
DatabaseConnectionContainer *cont = NULL;
ConnectionHandler *CH;
std::atomic<unsigned long> connCount;
Config *c;


void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		Log->WriteMessage(ERR,"SIGINT received stopping server!");
		delete(cont);
		delete(CH);
		delete(Log);
		delete(c);
		sem_destroy(&connectionSem);
	}
	exit(EXIT_SUCCESS);
}


	/**********************************************Start of ConnectionHandler section******************************************************/

/** Metoda pre nastavenie serveroveho socketu a pocuvanie na nom
    */
int ConnectionHandler::Listen ()  //funkcia na vytvorenie spojenia a komunikaciu s klientom
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Listen");
	//signal (SIGCHLD, endchild);
	/*struct sigaction act;
  
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
  
	sigaction(SIGCHLD, &act, 0);*/
	struct sockaddr_in sin;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)  //vytvorime socket 
	{
	  Log->WriteMessage(FATAL,"Creating socket failed");  //ak nastala chyba
	  Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return 1;
	}
	std::string opt;
	opt = "tap0";
	if (setsockopt(s, SOL_SOCKET, SO_BINDTODEVICE, opt.c_str(), 4) < 0)
	{
		Log->WriteMessage(ERR,"Unable to switch socket to VPN tap0");
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
		Log->WriteMessage(INFO,"Socket successfully binded");
	}
	if ((listen (s,10))<0) //chceme na sockete pocuvat 
	{
	  Log->WriteMessage(FATAL,"Unable to listen");
	  Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	  return 1;
	}
	else
	{
		Log->WriteMessage(INFO,"Listenin on port :" + std::to_string(_port));
	}
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Listen");
	return 0;
}


/** Metoda pre prijatie pripojenia na sockete servra
    */
int ConnectionHandler::ReciveConnection()
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
			std::thread worker(&ConnectionServer::HandleConnectionCover,CS,sin.sin_addr);
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

		//CS->HandleConnectionCover(sin.sin_addr);
	//close (com_s);  //zavreme socket
	}
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReciveConnection");
	return 0;  //vratime ze je vsetko v poriadku
}

/** Konstruktor objektu vytvoreneho z triedy ConnectionHandler
    */

ConnectionHandler::ConnectionHandler()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	_port = c->Port();
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

/** Destruktor objektu vytvoreneho z triedy ConnectionHandler */

ConnectionHandler::~ConnectionHandler()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	close(this->s);
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}

	/**********************************************End of ConnectionHandler section******************************************************/

	/**********************************************Start of ConnectionServer section******************************************************/

void ConnectionServer::HandleConnectionCover(in_addr IP)
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnectionCover");
	this->start = std::chrono::system_clock::now();
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
	this->HandleConnection(IP);
	cont->ReturnConnection(database->ReturnConnection());
	sem_post(&connectionSem);
	auto time = (end - start);
	Log->WriteMessage(TRACE,"Time to answer : " + std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(time).count()) + " microseconds");
	Log->WriteMessage(TRACE,"Process end!");
	connCount--;
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnectionCover");
	delete (this);
}


/** Metoda pre prijatie spravy a jej spracovanie a odpovedanie na nu
 * @param IP - ip adresa zariadenia s ktorym sa komunikuje
    */
void ConnectionServer::HandleConnection (in_addr IP)
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::HandleConnection");
	char buffer[1024];  //natavime buffer
	int pollRes;
	ssize_t DataSize=0;
	std::string data;
	data.clear();
	struct pollfd ufds;
	ufds.fd = com_s;
	ufds.events = POLLIN;
	while (1)
	{
		pollRes = poll(&ufds, 1, c->TimeOut()*1000);
		if(pollRes==-1)
		{
			Log->WriteMessage(WARN,"Reading data from client failed!");
			Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
			return;
		}
		if(pollRes==0)
		{
			Log->WriteMessage(WARN,"Connection to client timed out!");
			Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
			return;
		}
		else
		{
			if ((DataSize = read(this->com_s, &buffer, 1024)) < 0) //prijmeme dlzku prichadzajucich dat (2 byty kratkeho integera bez znamienka)
			{
				Log->WriteMessage(WARN,"Reading data from client failed");
				Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
				return;
			}
			if (DataSize==0)  //ak nic neprislo ukoncime proces
			{
				Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
				return;
			}
			data.append(buffer,DataSize);
			if ((data.find("</adapter_server>")!=std::string::npos)||((data[data.size()-2]=='/')&&(data[data.size()-2]=='>')))
			{
				break;
			}
		}
	}
	Log->WriteMessage(MSG,"Message :" + data);
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());
	if (result.status!=status_ok)
	{
		std::string tmp = "Wrong message XML format : ";
		tmp.append(result.description());
		Log->WriteMessage(WARN,tmp);
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		return;
	}
	xml_node adapter = doc.child("adapter_server");
	float CPversion =  adapter.attribute("protocol_version").as_float();
	int FMversion =  adapter.attribute("fw_version").as_int();
	if ((CPversion == (float)0.1)||(CPversion == (float)1.0))
	{
		if (FMversion == 0)
		{
			MP = new ProtocolV1MessageParser();
		}
		else
		{
			if (FMversion == 1)
			{
				try
				{
					MP = new ProtocolV1MessageParser();
				}
				catch (std::exception &e)
				{
					Log->WriteMessage(ERR,"Unable to create space for Protocol parser exiting client won't be server!");
					Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
					return;
				}
			}
			else
			{
				Log->WriteMessage(WARN,"Unsupported protocol version");
				Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
				return;
			}
		}
	}
	else
	{
		Log->WriteMessage(WARN,"Unsupported protocol version");
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		return;
	}
	if (!MP->parseMessage(&adapter,FMversion,CPversion))  //pomocou parsera spracujeme spravu
	{
		
		Log->WriteMessage(WARN,"Wrong request format");
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
		return;
	}
	MP->setAdapterIP(IP);  //ulozime IP adresu adapteru do spravy
	parsedMessage = MP->ReturnMessage();  //spracovanu spravu si ulozime lokalne
	if (parsedMessage->state!=0)
	{
		response = MP->CreateAnswer(this->GetData());  //zavolame metodu na vytvorenie odpovede a metodu na ulozenie dat do DB
		int Err;
		Log->WriteMessage(MSG,"Message: \n" + response);
		if ((Err=send(this->com_s, response.c_str(), response.size() , 0))<0)  //odoslanie odpovede klientovi
		{
			Log->WriteMessage(WARN,"Unable to send answer");
		}
	}
	this->end = std::chrono::system_clock::now();
	this->StoreData();
	if (parsedMessage->state!=0)
	{
		database->LogValue(parsedMessage); //zavolame metodu ktora zisti ci je pre dane zariadenie zapnute logovanie hodnot a pripadne ich ulozi
	}
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::HandleConnection");
	return;
}

ConnectionServer::ConnectionServer(int s)
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->com_s = s;
	this->MP = NULL;
	this->database = NULL;
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

ConnectionServer::~ConnectionServer()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	close(this->com_s);
	delete (this->MP);
	delete this->database;
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}



/** Metoda volajuca funkcie na ulozenie a ziskanie dat z DB
 * @param message - spracovana sprava
    */

void ConnectionServer::StoreData()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::StoreData");
	if (this->parsedMessage->state == 0)
	{
		database->InsertAdapter(this->parsedMessage);
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::StoreData");
		return;
	}
	else
	{
		database->UpdateAdapter(this->parsedMessage); //ak ano uz len updatneme jeho data
		if(!database->IsInDB("facilities","mac","'" + this->parsedMessage->DeviceIDstr + "'")) //to iste spravime aj pre senzor/aktor
		{
			database->InsertSenAct(this->parsedMessage);
		}
		else
		{
			database->UpdateSenAct(this->parsedMessage);
		}
	}
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::StoreData");
}

int ConnectionServer::GetData()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetData");
	int wakeUpTime = 5;
	if(!database->IsInDB("facilities","mac","'" + this->parsedMessage->DeviceIDstr + "'"))
	{
		wakeUpTime = 5;
	}
	else
	{
		wakeUpTime = database->GetWakeUpTime(this->parsedMessage->DeviceIDstr);
	}
	Log->WriteMessage(TRACE,"refresh:" + std::to_string(wakeUpTime));
	if(wakeUpTime> 100000)
		wakeUpTime = 10;
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetData");
	return wakeUpTime;
}


	/**********************************************End of ConnectionServer section******************************************************/


	/**********************************************Start of MessageParser section******************************************************/

/** Metoda vracajuca obsah spracovanej spravy
 * @return _message
    */



tmessage* MessageParser::ReturnMessage()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::ReturnMessage");
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::ReturnMessage");
	return _message;

}

	/**********************************************End of MessageParser section******************************************************/

	/**********************************************Start of ProtocolV1MessageParser section******************************************************/


/** Metoda spracovanie spravy od adaptera verzie protokolu 1
 * @param *message - ukazatel na zaciatok prijatej spravy
 * @param length - dlzka spravy
 * @return na zaklade uspechu/neuspechu vrati true/false
    */



bool ProtocolV1MessageParser::parseMessage(xml_node *adapter,float FM,float CP)
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::parseMessage");
	this->_adapter = adapter;
	this->_device = this->_adapter->child("device");
	//ulozime si ID adaptera v znakovej forme
	this->GetID();
	//uloznie verzie firmwaru 
	this->_message->cp_version=CP;
	Log->WriteMessage(INFO,"CP version " + std::to_string(_message->cp_version));
	//ulozenie verzie komunikacneho protokolu
	this->_message->fm_version=FM;
	Log->WriteMessage(INFO,"FM version " + std::to_string(_message->fm_version));
	this->GetState();
	//spracujeme a ulozime casove razitko
	this->GetTimeStamp();
	if (_message->state == 0)
	{
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::parseMessage");
		return true;
	}
	//ulozime si IP adresu zariadenia (verzia pre emulatory)
	this->GetDeviceID();
	
	//ulozime inicializacnu hodnotu
	//this->GetInitVal();
	
	//ulozime si verziu komunikacneho protokolu zariadenia
	//this->GetDeviceCP();

	//ulozime hodnotu baterie

	this->GetBattery();

	//ulozime hodnotu kvality signalu

	this->GetSignal();
	//ulozime pocet posielanych dvojic typ hodnota

	this->GetValues();
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::parseMessage");
	return true;
}

void ProtocolV1MessageParser::GetTimeStamp()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetTimeStamp");
	time_t tmpTime = 0;

	tmpTime = 0;
    //double tempd = 0.0; //pretoze python pouziva na ulozenie casoveho razitka datovy typ s plavajucou desatinnou ciarkou musime vykonat pretypovanie
	this->_message->timestamp = _adapter->attribute("time").as_llong();
	char timebuf[200];
	struct tm *tmp;
	tmp = gmtime(&(this->_message->timestamp));
	Log->WriteMessage(MSG,"Timestamp int val:"+ std::to_string(tmpTime) + " UTC");
	strftime(timebuf,sizeof(timebuf),"%Y-%m-%d %T",tmp);
	std::string msg;
	msg.append(timebuf);
	Log->WriteMessage(MSG,"Timestamp :"+ msg + " UTC");
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetTimeStamp");
}

void ProtocolV1MessageParser::GetID()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetID");
	this->_message->adapterINTid = std::stoll(_adapter->attribute("adapter_id").as_string(),nullptr,16);
    Log->WriteMessage(MSG,"Adapter int ID:" + std::to_string(this->_message->adapterINTid));
    Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetID");
}

void ProtocolV1MessageParser::GetFM()
{

}

void ProtocolV1MessageParser::GetCP()
{

	
}

void ProtocolV1MessageParser::GetState()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetState");
	std::string temp_state = _adapter->attribute("state").as_string();

	if (temp_state.compare("register")==0)
	{
		_message->state = 0;
	}
	else
	{
		if (temp_state.compare("data")==0)
		{
			_message->state = 1;
		}
		else
		{
			_message->state = 2;
		}
	}
	Log->WriteMessage(MSG,"STATE :" + std::to_string(this->_message->state));
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetState");
}

void ProtocolV1MessageParser::GetDeviceID()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetDeviceID");
	this->_message->sensor_id = std::stoll(_device.attribute("id").as_string(),nullptr,16);
	in_addr_t temp = htonl (_message->sensor_id);
	Log->WriteMessage(MSG,"Device id :" + std::to_string(this->_message->sensor_id));
	struct sockaddr_in antelope;
	antelope.sin_addr.s_addr = temp;
	this->_message->DeviceIDstr = inet_ntoa (antelope.sin_addr);
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetDeviceID");
}

void ProtocolV1MessageParser::GetInitVal()
{

	//Log->WriteMessage(INFO,"Init Val :" + std::to_string(this->_message->init));
}

void ProtocolV1MessageParser::GetDeviceCP()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetDeviceCP");
	Log->WriteMessage(INFO,"Device protocol version :" + std::to_string(this->_message->deviceProcolVersion));
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetDeviceCP");
}


void ProtocolV1MessageParser::GetBattery()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetBattery");
	xml_node battery = (_device.child("battery"));
	_message->battery=battery.attribute("value").as_uint();
	Log->WriteMessage(MSG,"Sensor battery :" + std::to_string(_message->battery));
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetBattery");
}
void ProtocolV1MessageParser::GetSignal()
{
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetSignal");
	xml_node signal = (_device.child("rssi"));
	_message->signal_strength=signal.attribute("value").as_uint();
	Log->WriteMessage(MSG,"Sensor signal :" + std::to_string(_message->signal_strength));
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetSignal");
}
bool ProtocolV1MessageParser::GetValues()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetValues");
	//vytvotime si v pamati miesto na ulozenie dvojic typ hodnota
	xml_node values = _device.child("values");
	_message->values_count = values.attribute("count").as_uint();
	xml_node value = values.first_child();
	try
	{
		_message->values = new tvalue[_message->values_count];
	}
	catch(std::exception &e)
	{
		Log->WriteMessage(ERR,"Unable to allocate memory for values client won't be served properly!" );
		_message->values = NULL;
		Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetValues");
		return false;
	}
	for (int i = 0; i < _message->values_count; i++) //v cykle spracujeme prijate dvojice typ hodnota
	{
		if (value==NULL)
		{
			_message->values_count = i;
			break;
		}
		_message->values[i].intType=value.attribute("type").as_uint();
		_message->values[i].type = static_cast<tvalueTypes>(_message->values[i].intType);
		Log->WriteMessage(MSG,"Type :" + std::to_string(_message->values[i].type));
		if ((_message->values[i].type==ONOFFSEN)||(_message->values[i].type==ONOFSW))  //ak je typ 0x04 a 0x05 precitame jeden byte a ulozime hodnotu
		{
			_message->values[i].bval = false;
			if (strcmp(value.value(),"1")==0)
				_message->values[i].bval = true;
			Log->WriteMessage(MSG,"Value :" + std::to_string(_message->values[i].bval));
			if (_message->devType==UNDEF)
			{
				if(_message->values[i].type==ONOFFSEN)
					_message->devType=SEN;
				else
					_message->devType=ACT;
			}
			else
			{
				if (((_message->devType==ACT)&&(_message->values[i].type==ONOFFSEN))||((_message->devType==SEN)&&(_message->values[i].type==ONOFSW)))
				{
					_message->devType=SENACT;
				}
			}

		}
		else
		{
			_message->values[i].fval = value.text().as_float();
			Log->WriteMessage(MSG,"Value :" + std::to_string(_message->values[i].fval));
			if (_message->devType==UNDEF)
			{
				_message->devType=SEN;
			}
			else
			{
				if (_message->devType==ACT)
				{
					_message->devType=SENACT;
				}
			}
		}
	value = value.next_sibling();
	}
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetValues");
	return true;
}
void ProtocolV1MessageParser::GetValuesCount()
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::GetValuesCount");
	Log->WriteMessage(MSG,"Values count :" + std::to_string(_message->values_count));
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::GetValuesCount");
}

/** Metoda vytvarajuca odpoved adapteru
 * @param value - hodnota dalsieho zobudenia senzoru
 * @return resp - ukazatel na spracovanu spravu
    */

std::string ProtocolV1MessageParser::CreateAnswer(int value)
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::CreateAnswer");
	xml_document *resp = new xml_document();
	xml_node server_adapter = resp->append_child("server_adapter");
	//server_adapter->set_name("server_adapter");
	server_adapter.append_attribute("protocol_version");
	server_adapter.append_attribute("state");
	server_adapter.append_attribute("id");
	server_adapter.append_attribute("time");
	std::stringstream s;
	s.precision(2);
	s << _message->cp_version;
	server_adapter.attribute("protocol_version") = s.str().c_str();
	server_adapter.attribute("state") = "update";
	server_adapter.attribute("id") = std::to_string(_message->sensor_id).c_str();
	server_adapter.attribute("time") = std::to_string(value).c_str();
	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	std::string tmp = writer.result;
	if (tmp[tmp.length()-1]=='\n')
	{
		tmp.erase(tmp.length()-1,1);
	}
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::CreateAnswer");
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + tmp;
};


ProtocolV1MessageParser::~ProtocolV1MessageParser() 
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Destructor");
	delete(this->_message);
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Destructor");
}
ProtocolV1MessageParser::ProtocolV1MessageParser() 
{
	Log->WriteMessage(TRACE,"Entering " + this->_Name + "::Constructor");
	this->_message = new tmessage();
	Log->WriteMessage(TRACE,"Exiting " + this->_Name + "::Constructor");
}

	/**********************************************End of ProtocolV1MessageParser section******************************************************/



	/**********************************************Start of message section*********************************************************/

message::message()
{
	values = NULL;
	devType = UNDEF;
}

message::~message()
{
	delete [] values;
	values= NULL;
}

	/**********************************************End of message section**********************************************************/


int main()  //hlavne telo programu
{
	try
	{
		c = new Config();
	}
	catch(std::exception &e)
	{
		std::cerr<<e.what()<<std::endl;
		std::cerr<<"Unable to create memory space for Configuration exiting!"<<std::endl;
		exit(EXIT_FAILURE);
	}
	connCount = 0;
	try
	{
		Log = new Loger();
	}
	catch(std::exception &e)
	{
		std::cerr<<e.what()<<std::endl;
		std::cerr<<"Unable to create memory space for logging exiting!"<<std::endl;
		exit(EXIT_FAILURE);
	}
	if (!c->setConfig("ada_server_receiver.config.xml",0))
	{
		return 1;
	}
	else
	{
		Log->SetLogger(c->Verbosity(),c->MaxFiles(),c->MaxLines(),c->FileNaming(),"ADA SERVER RECEIVER");
	}
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		Log->WriteMessage(ERR,"Unable to catch SIGINT");
	}
	cont = DatabaseConnectionContainer::CreateContainer(Log,c->DBName(),c->ConnLimit());
	int semVal = cont->Limit();
	Log->WriteMessage(FATAL,"Maximal conn count : " + std::to_string(semVal));
	sem_init(&connectionSem,0,semVal);
	try
	{
		CH = new ConnectionHandler();
	}
	catch(std::exception &e)
	{
		std::cerr<<e.what()<<std::endl;
		std::cerr<<"Unable to create memory space for ConnectionHandler exiting!"<<std::endl;
		exit(EXIT_FAILURE);
	}
	CH->Listen();  //zacneme pocuvat na porte
	CH->ReciveConnection();  //zacneme prijimat a obsluhovat pripojenia
	//delete (CH);
	delete(cont);
	delete (CH);
	delete (c);
	delete (Log);
	return 0;
}
