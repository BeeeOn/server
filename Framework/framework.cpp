/**
* @file framework.cpp
*
* @Implementace modulárního prostøedí pro algoritmy
*
* @author xrasov01
* @version 1.0
*/

#include "framework.h"

#define BUFSIZE 1000
#define ADA_SERVER_PORT 7383
#define UI_SERVER_PORT 1111

using namespace std;
using namespace soci;
using namespace pugi;

FrameworkServer *Server;					//Ukazatel na instanci serveru Frameworku
Loger *Log;									//Loger pro logování do souboru
DatabaseConnectionContainer *cont = NULL;	//Container pro DB
sem_t connectionSem;						//Semafor pro maximální poèet pøipojení k Frameworku
int counter = 0;							//Counter pro notifikator

int FrameworkServer::SetUpSockets(){

	//Vytvoøení soketu
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		cerr << "Nelze vytvoøit soket" << endl;
		return -1;
	}

	//Nastavení struktury portBind
	portBind.sin_family = AF_INET;
	portBind.sin_port = htons(port);
	portBind.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (sockaddr *)&portBind, sizeof(portBind)) == -1)
	{
		cerr << "Problém s pojmenováním soketu." << endl;
		return -1;
	}
	// Vytvoøíme frontu požadavkù na spojení. 
	// Vytvoøíme frontu maximální velikosti 10 požadavkù.
	if (listen(serverSocket, 10) == -1)
	{
		cerr << "Problém s vytvoøením fronty" << endl;
		return -1;
	}
	return 0;
}

int FrameworkServer::AcceptConnection(){

	sockaddr_in clientInfo;			// Informace o klientovi
	socklen_t addrlen;				// Velikost adresy vzdáleného poèítaèe
	addrlen = sizeof(clientInfo);
	int newSocket;					//Socket spojení s klientem					

	while (1)
	{
		if ((newSocket = accept(serverSocket, (sockaddr*)&clientInfo, &addrlen)) < 0)  //budeme na nom prijimat data
		{
			cerr << "Problém s pøijetím spojeni" << endl;
			return 1;
		}
		sem_wait(&connectionSem);

		try
		{
			FrameworkServerHandle *FSH = new FrameworkServerHandle(newSocket, ntohs(clientInfo.sin_port));
			std::thread worker(&FrameworkServerHandle::HandleClientConnection, FSH);
			worker.detach();
		}
		catch (std::bad_alloc &e)
		{
			cerr << "Chyba alokace " << endl;
			close(newSocket);
		}
		catch (std::exception &e)
		{
			cerr << "Client won't be served unable to crate thread! " << endl;
			close(newSocket);
		}
	}
	return 0;  //vratime ze je vsetko v poriadku
}

FrameworkServer::FrameworkServer(int init_port){
	this->port = init_port;
}

FrameworkServer::~FrameworkServer(){
	close(this->serverSocket);
}

//FrameworkServerHandle

void FrameworkServerHandle::HandleClientConnection(){

	//Vytvoøení spojení s DB
	session *Conn = NULL;
	while (Conn == NULL)
	{
		Conn = cont->GetConnection();
		if (Conn == NULL)
		{
			Log->WriteMessage(INFO, "ALL CONNECTIONS ARE USED!");
		}
	}
	try
	{
		this->database = new DBHandler(Conn, Log);
	}
	catch (std::exception &e)
	{
		Log->WriteMessage(ERR, "Client won't be served unable to create DBHandler!");
		close(this->handledSocket);
		Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnectionCover");
		sem_post(&connectionSem);
		delete (this);
		return;
	}

	char buf[BUFSIZE];				// Pøijímací buffer
	int size;						// Poèet pøijatých a odeslaných bytù
	int pollRes;
	ssize_t DataSize = 0;
	std::string data;
	data.clear();
	struct pollfd ufds;
	ufds.fd = handledSocket;
	ufds.events = POLLIN;


	//TEST
	cerr << clientPort ; //takto zjistim port


	bool isAdapterServerMessage = false;
	bool uiServerMessage = false;
	bool isAlgorithmMessage = false;

	while (1)
	{
		pollRes = poll(&ufds, 1, 100000); // 3. parametr je Timeout
		if (pollRes == -1)
		{
			cerr << "Reading data from client failed!" << endl;
			return;
		}
		if (pollRes == 0)
		{
			cerr << "Connection to client timed out!" << endl;
			return;
		}
		else
		{
			if ((DataSize = read(this->handledSocket, &buf, 1024)) < 0) //prijmeme dlzku prichadzajucich dat (2 byty kratkeho integera bez znamienka)
			{
				cerr << "Problem with reading data!" << endl;
				return;
			}

			if (clientPort == ADA_SERVER_PORT){
				isAdapterServerMessage = true;
				if (DataSize == 0)
				{
					cerr << "Nothing to be read! DataSize==0" << endl;
					return;
				}
				data.append(buf, DataSize);
				if ((data.find("</adapter_server>") != std::string::npos) || ((data[data.size() - 2] == '/') && (data[data.size() - 2] == '>')))
				{
					break;
				}
			}
			else if (clientPort == UI_SERVER_PORT){
				uiServerMessage = true;

			}
			else{
				//Zde je pøijímání zpráv od algoritmù
				isAlgorithmMessage = true;
				
				if (DataSize == 0)
				{
					cerr << "Nothing to be read! DataSize==0" << endl;
					return;
				}
				data.append(buf, DataSize);
				if ((data.find("</algorithm_message>") != std::string::npos) || ((data[data.size() - 2] == '/') && (data[data.size() - 2] == '>')))
				{
					break;
				}
			}
		}
	}

	//zaèíná parser zprávy
	if (isAdapterServerMessage){
		cerr << "AdapterServerMessage" << endl;
		xml_document doc;
		xml_parse_result result = doc.load_buffer(data.data(), data.size());
		xml_node adapter = doc.child("adapter_server");
		float CPversion = adapter.attribute("protocol_version").as_float();
		int FMversion = adapter.attribute("fw_version").as_int();

		if ((CPversion == (float)0.1) || (CPversion == (float)1.0))
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
						Log->WriteMessage(ERR, "Unable to create space for Protocol parser exiting client won't be server!");
						Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
						return;
					}
				}
				else
				{
					try
					{
						MP = new ProtocolV1MessageParser();
					}
					catch (std::exception &e)
					{
						Log->WriteMessage(ERR, "Unable to create space for Protocol parser exiting client won't be server!");
						Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
						return;
					}
				}
			}
		}
		else
		{
			Log->WriteMessage(WARN, "Unsupported protocol version");
			Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
			return;
		}
		if (!MP->parseMessage(&adapter, FMversion, CPversion))  //pomocou parsera spracujeme spravu
		{
	
			Log->WriteMessage(WARN, "Wrong request format");
			Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleConnection");
			return;
		}
		//this->MP->setAdapterIP(IP);  //ulozime IP adresu adapteru do spravy ODSTRANENO PROTOZE nepotøebuji IP adresu
		parsedMessage = this->MP->ReturnMessage();
		if (parsedMessage->state != 0)
		{

			//  The argument list to pass to the Algorithm
			char* arg_list[] = {
				"alg1",     /* argv[0], the name of the program.  */
				"-u",
				"1",
				"-a",
				"1",
				"-d",
				"1",
				"-o",
				"1",
				"/",
				NULL      /* The argument list must end with a NULL.  */
			};

			this->spawn("Algorithms/alg1", arg_list);

		}
	}
	else if (uiServerMessage){
		//Kód zpracující zprávy od UIServeru (Pøeposílání zpráv od uživatelù Androidu)
		

	}
	else{
		//Kód zpracující zprávy od Algoritmù

		cerr << "Framework Info: Algorithm send me a message!" << endl;
	}

	sem_post(&connectionSem);
	delete(this);
	return;
}

int FrameworkServerHandle::spawn(char* program, char** arg_list)
{
	pid_t child_pid;

	/* Duplicate this process. */
	child_pid = fork();
	if (child_pid != 0)
		/* This is the parent process. */
		return child_pid;
	else {
		/* Now execute PROGRAM, searching for it in the path. */
		execvp(program, arg_list);
		/* The execvp  function returns only if an error occurs. */
		fprintf(stderr, "an error occurred in execvp\n");
		abort();
	}
}

//Metoda zpracujici hodnoty ze zpravy
void FrameworkServerHandle::Watchdog(){
	Log->WriteMessage(TRACE, "Entering " + this->_Name + "::Notify");
	for (int i = 0; i<this->parsedMessage->values_count; i++)
	{
		if (this->parsedMessage->values[i].type != TEMP)
		{
			continue;
		}
		else
		{
			if (this->parsedMessage->values[i].fval<30.0)
			{
				continue;
			}
			else
			{
				float lastVal = this->database->GetLastTemp(this->parsedMessage->DeviceIDstr, std::to_string(this->parsedMessage->values[i].intType));
				Log->WriteMessage(TRACE, "Last val : " + std::to_string(lastVal));
				if (lastVal>30.0)
				{
					continue;
				}
				else
				{
					std::vector<std::string> *emails;
					emails = database->GetEmails(std::to_string(this->parsedMessage->adapterINTid));
					if (emails == nullptr)
					{
						continue;
					}
					else
					{
						for (int i = 0; i<emails->size(); i++)
						{
							if (emails->at(i).empty())
							{
								break;
							}
							std::vector<std::string> *IDs;
							IDs = database->GetNotifString(emails->at(i));
							if (IDs == nullptr)
							{
								Log->WriteMessage(INFO, "Email " + emails->at(i) + " has no notification string");
								continue;
							}
							Log->WriteMessage(INFO, "Notifying email " + emails->at(i));
							Notification *notif = new LimitExceededNotification(emails->at(i), counter++, *IDs, this->parsedMessage->timestamp, "Temperature exceeded 30 degrees on adapter :" + std::to_string(this->parsedMessage->adapterINTid) + " on device " + std::to_string(this->parsedMessage->sensor_id), 123, this->parsedMessage->DeviceIDstr, 1, 2);
							Notificator::sendNotification(*notif);
							delete IDs;
							delete notif;
						}
					}
				}
			}
		}
	}
	Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::Notify");
}
FrameworkServerHandle::FrameworkServerHandle(int init_socket, uint16_t init_port){
	this->handledSocket = init_socket;
	this->clientPort = init_port;
}

FrameworkServerHandle::~FrameworkServerHandle(){
	close(this->handledSocket);
}



/**********************************************Start of MessageParser section******************************************************/

/** Metoda vracajuca obsah spracovanej spravy
* @return _message
*/



tmessage* MessageParser::ReturnMessage()
{
	return _message;

}

/**********************************************End of MessageParser section******************************************************/

/**********************************************Start of ProtocolV1MessageParser section******************************************************/


/** Metoda spracovanie spravy od adaptera verzie protokolu 1
* @param *message - ukazatel na zaciatok prijatej spravy
* @param length - dlzka spravy
* @return na zaklade uspechu/neuspechu vrati true/false
*/



bool ProtocolV1MessageParser::parseMessage(xml_node *adapter, float FM, float CP)
{
	this->_adapter = adapter;
	this->_device = this->_adapter->child("device");
	//ulozime si ID adaptera v znakovej forme
	this->GetID();
	//uloznie verzie firmwaru 
	this->_message->cp_version = CP;
	//ulozenie verzie komunikacneho protokolu
	this->_message->fm_version = FM;
	this->GetState();
	//spracujeme a ulozime casove razitko
	this->GetTimeStamp();
	if (_message->state == 0)
	{
		return true;
	}
	//ulozime si IP adresu zariadenia (verzia pre emulatory)
	this->GetDeviceID();
	this->GetBattery();

	//ulozime hodnotu kvality signalu
	this->GetSignal();
	//ulozime pocet posielanych dvojic typ hodnota
	this->GetValues();
	return true;
}

void ProtocolV1MessageParser::GetTimeStamp()
{
	time_t tmpTime = 0;

	tmpTime = 0;
	//double tempd = 0.0; //pretoze python pouziva na ulozenie casoveho razitka datovy typ s plavajucou desatinnou ciarkou musime vykonat pretypovanie
	this->_message->timestamp = _adapter->attribute("time").as_llong();
	char timebuf[200];
	struct tm *tmp;
	tmp = gmtime(&(this->_message->timestamp));
	strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %T", tmp);
	std::string msg;
	msg.append(timebuf);
}

void ProtocolV1MessageParser::GetID()
{
	this->_message->adapterINTid = std::stoll(_adapter->attribute("adapter_id").as_string(), nullptr, 16);
}

void ProtocolV1MessageParser::GetFM()
{

}

void ProtocolV1MessageParser::GetCP()
{


}

void ProtocolV1MessageParser::GetState()
{
	std::string temp_state = _adapter->attribute("state").as_string();

	if (temp_state.compare("register") == 0)
	{
		_message->state = 0;
	}
	else
	{
		if (temp_state.compare("data") == 0)
		{
			_message->state = 1;
		}
		else
		{
			_message->state = 2;
		}
	}
}

void ProtocolV1MessageParser::GetDeviceID()
{
	this->_message->sensor_id = std::stoll(_device.attribute("id").as_string(), nullptr, 16);
	in_addr_t temp = htonl(_message->sensor_id);
	struct sockaddr_in antelope;
	antelope.sin_addr.s_addr = temp;
	this->_message->DeviceIDstr = inet_ntoa(antelope.sin_addr);
}


void ProtocolV1MessageParser::GetBattery()
{
	xml_node battery = (_device.child("battery"));
	_message->battery = battery.attribute("value").as_uint();
}
void ProtocolV1MessageParser::GetSignal()
{
	xml_node signal = (_device.child("rssi"));
	_message->signal_strength = signal.attribute("value").as_uint();
}
bool ProtocolV1MessageParser::GetValues()
{
	//vytvotime si v pamati miesto na ulozenie dvojic typ hodnota
	xml_node values = _device.child("values");
	_message->values_count = values.attribute("count").as_uint();
	xml_node value = values.first_child();
	try
	{
		_message->values = new tvalue[_message->values_count];
	}
	catch (std::exception &e)
	{
		_message->values = NULL;
		return false;
	}
	for (int i = 0; i < _message->values_count; i++) //v cykle spracujeme prijate dvojice typ hodnota
	{
		if (value == NULL)
		{
			_message->values_count = i;
			break;
		}
		unsigned short int tempType = value.attribute("type").as_uint();
		_message->values[i].offset = value.attribute("offset").as_uint();
		tconcatenate temp;
		temp.input[0] = tempType;
		temp.input[1] = _message->values[i].offset;
		_message->values[i].intType = temp.result;
		_message->values[i].type = static_cast<tvalueTypes>(tempType);
		switch (_message->values[i].type)
		{
		case TEMP:
		case LUM:
		case REZ:
		case POS:
			_message->values[i].fval = value.text().as_float();
			if (_message->devType == UNDEF)
			{
				_message->devType = SEN;
			}
			else
			{
				if (_message->devType == ACT)
				{
					_message->devType = SENACT;
				}
			}
			break;
		case ONON:
		case TOG:
		case ONOFFSEN:
		case ONOFSW:
			_message->values[i].bval = false;
			if (value.text().as_float() == 1.0)
				_message->values[i].bval = true;
			if (_message->devType == UNDEF)
			{
				if (_message->values[i].type == ONOFFSEN)
					_message->devType = SEN;
				else
					_message->devType = ACT;
			}
			else
			{
				if (((_message->devType == ACT) && (_message->values[i].type == ONOFFSEN)) || ((_message->devType == SEN) && (_message->values[i].type == ONOFSW)))
				{
					_message->devType = SENACT;
				}
			}
			break;
		case EMI:
		case HUM:
		case BAR:
		case RGB:
		case RAN:
			_message->values[i].ival = value.text().as_int();
			if (_message->devType == UNDEF)
			{
				_message->devType = SEN;
			}
			else
			{
				if (_message->devType == ACT)
				{
					_message->devType = SENACT;
				}
			}
			break;
		default:
			_message->values[i].type = UNK;
			break;
		}
		value = value.next_sibling();
	}
	return true;
}
void ProtocolV1MessageParser::GetValuesCount()
{
}

/** Metoda vytvarajuca odpoved adapteru
* @param value - hodnota dalsieho zobudenia senzoru
* @return resp - ukazatel na spracovanu spravu
*/

std::string ProtocolV1MessageParser::CreateAnswer(int value)
{

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
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + tmp;
};


ProtocolV1MessageParser::~ProtocolV1MessageParser()
{
	delete(this->_message);
}
ProtocolV1MessageParser::ProtocolV1MessageParser()
{
	this->_message = new tmessage();
}

/**********************************************End of ProtocolV1MessageParser section******************************************************/



/**********************************************Start of message section*********************************************************/

message::message()
{
	this->values = NULL;
	this->devType = UNDEF;
	this->DeviceIDstr = "";
	this->adapterINTid = 0;
	inet_pton(AF_INET, "0.0.0.0", &(this->adapter_ip));
	this->battery = 0;
	this->cp_version = 0.0;
	this->signal_strength = 0;
	this->state = 0;
	this->values_count = 0;
	this->fm_version = 0;
	this->sensor_id = 0;
	this->timestamp = 0;

}

message::~message()
{
	delete[] values;
	values = NULL;
}

/**********************************************End of message section**********************************************************/

void sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		delete(Server);
		delete(cont);
		sem_destroy(&connectionSem);
		delete(Log);
	}
	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
	if (signal(SIGINT, sig_handler) == SIG_ERR)
	{
		std::cerr << "Unable to catch SIGINT";
	}
	//Loger pro logování do souboru
	try
	{
		Log = new Loger();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "Unable to create memory space for logging exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}
	//Nastaveni kontejneru pro DB
	cont = DatabaseConnectionContainer::CreateContainer(Log, "home4", 10);
	//Nastaveni semaforu pro omezeni poctu pripojeni k DB
	int semVal = 10;
	Log->WriteMessage(FATAL, "Maximal conn count : " + std::to_string(semVal));
	sem_init(&connectionSem, 0, semVal);
	//Nastaveni Loggeru
	Log->SetLogger(7, 5, 100, "framework_log", "FRAMEWORK");
	//Instanciace serveru
	try
	{
		Server = new FrameworkServer(7382);
	}
	catch (std::exception &e)
	{
		std::cerr << "Framework: Unable to create memory space for FrameworkServer. Exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}
	Server->SetUpSockets();
	Server->AcceptConnection();

	delete(Server);

	return 0;
}



