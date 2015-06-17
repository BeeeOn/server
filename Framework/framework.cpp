/**
* @file framework.cpp
*
* Implementace modulárního prostøedí pro algoritmy
*
* @author xrasov01
* @version 1.0
*/

#include "framework.h"

//Konstanty
#define UI_FRAMEWORK_PORT 7082 
#define ADA_FRAMEWORK_PORT 7083 
#define ALG_FRAMEWORK_PORT 7084
#define BUFSIZE 1000

//Namespacy
using namespace std;
using namespace soci;
using namespace pugi;
namespace sc = std::chrono;

// Globalni promenne
FrameworkServer * UIServer;					
FrameworkServer * AdaServer;
FrameworkServer * AlgServer;
Loger *Log;									//Loger pro logování do souboru
DBConnectionsContainer *cont = NULL;		//Container pro DB
FrameworkConfig *FConfig = NULL;
sem_t connectionSem;						//Semafor pro maximální poèet pøipojení k Frameworku
sem_t dbAccessSem;
std::atomic<unsigned long> connCount;

//---------------------------------------  BEGIN FRAMEWORK SERVER METHODS IMPLEMENTATIONS ---------------------------------------

FrameworkServer::FrameworkServer(int init_port){
	this->port = init_port;
}

FrameworkServer::~FrameworkServer(){
	close(this->serverSocket);
}

/**
* Zacne naslouchat na drive specifikovanem portu v instanciaci objektu FrameworkServer 
* 
*/
void FrameworkServer::StartServer(){
	this->SetUpSockets();
	this->AcceptConnection();
}

/** Konstruktor nastavi sockety pro naslouchani serveru
*
*/
bool FrameworkServer::SetUpSockets(){

	//Vytvoøení soketu
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		cerr << "Nelze vytvoøit soket" << endl;
		return false;
	}

	//Nastavení struktury portBind
	portBind.sin_family = AF_INET;
	portBind.sin_port = htons(port);
	portBind.sin_addr.s_addr = INADDR_ANY;

	if (bind(serverSocket, (sockaddr *)&portBind, sizeof(portBind)) == -1)
	{
		cerr << "Problém s pojmenováním soketu." << endl;
		return false;
	}
	// Vytvoøíme frontu požadavkù na spojení. 
	// Vytvoøíme frontu maximální velikosti 10 požadavkù.
	if (listen(serverSocket, 10) == -1)
	{
		cerr << "Problém s vytvoøením fronty" << endl;
		return false;
	}
	return true;
}

/** Metoda reprezentujici konkruentni server naslouchajici na portu definovanem v Objektu FrameworkServer a vytvarejici vlakno pri prichozim spojeni, ktere obsluhuje prichozi spojeni.
*
*/
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
		connCount++;
		Log->WriteMessage(INFO, "CONNECTION COUNT : " + std::to_string(connCount));
		try
		{
			FrameworkServerHandle *FSH = new FrameworkServerHandle(newSocket, this->port);
			std::thread worker(&FrameworkServerHandle::HandleClientConnection, FSH);
			worker.detach();
		}
		catch (std::bad_alloc &e)
		{
			Log->WriteMessage(ERR, "Allocation error FrameworkServerHandle !");
			close(newSocket);
		}
		catch (std::exception &e)
		{
			Log->WriteMessage(ERR, "Client won't be served unable to crate thread!");
			close(newSocket);
		}
	}
	return 0;
}
//---------------------------------------  END FRAMEWORK SERVER METHODS IMPLEMENTATIONS ---------------------------------------

//---------------------------------------  BEGIN FRAMEWORK SERVER HANDLE METHODS IMPLEMENTATIONS ---------------------------------------


/** Metoda obsluhujici prichozi spojeni. Je rozdelena na tri casti, protoze je implementovana k obsluze tri serveru (3 portu). Tyto tri servery jsou: Server pro zpracovani zprav od UI serveru, Server pro zpracovani zprav od Adapter Reciever serveru a Server pro zpracovani zprav od Algoritmu
*
*/
void FrameworkServerHandle::HandleClientConnection(){
	Log->WriteMessage(TRACE, "Entering HandleClientConnection");
	bool isAdapterServerMessage = false;
	bool isUIServerMessage = false;
	bool isAlgorithmMessage = false;

	//Obsluha zprav od UI serveru
	if (this->port == FConfig->portUIServer){
		isUIServerMessage = true;
	}
	//Obsluha zprav od Adapter Reciever serveru
	else if (this->port == FConfig->portAdaRecieverServer){
		isAdapterServerMessage = true;
	}
	//Obsluha zprav od Algoritmu (Aplikacnich modulu)
	else if (this->port == FConfig->portAlgorithmServer){
		isAlgorithmMessage = true;
	}
	else{
		Log->WriteMessage(ERR, "Something is wrong with Server port!");
	}
	//Ziskani spojení s DB
	session *Conn = NULL;

	Conn = cont->GetConnection();
	if (Conn == NULL)
	{
		Log->WriteMessage(INFO, "ALL CONNECTIONS ARE USED! Exiting thread!");
		connCount--;
		sem_post(&connectionSem);
		delete(this);
	}
	try
	{
		this->database = new DBFWHandler(Conn, Log);
	}
	catch (std::exception &e)
	{
		Log->WriteMessage(ERR, "Client won't be served unable to create DBFWHandler!");
		close(this->handledSocket);
		Log->WriteMessage(TRACE, "Exiting " + this->_Name + "::HandleClientConnection");
		sem_post(&connectionSem);
		connCount--;
		delete (this);
		return;
	}
	char buf[FConfig->recieveBuffSize];// Prijimaci buffer
	int pollRes;
	ssize_t DataSize = 0;
	std::string data;
	data.clear();
	struct pollfd ufds;
	ufds.fd = handledSocket;
	ufds.events = POLLIN;
	while (1)
	{
		pollRes = poll(&ufds, 1, 1000); // 3. parametr je Timeout
		if (pollRes == -1)
		{
			Log->WriteMessage(ERR, "Reading data from client failed!");
			connCount--;
			sem_post(&connectionSem);
			delete(this);
			return;
		}
		if (pollRes == 0)
		{
			Log->WriteMessage(ERR, "Connection to client timed out!");
			connCount--;
			sem_post(&connectionSem);
			delete(this);
			return;
		}
		else
		{
			if ((DataSize = read(this->handledSocket, &buf, 1024)) < 0)
			{
				Log->WriteMessage(ERR, "Problem with reading data!");
				connCount--;
				sem_post(&connectionSem);
				delete(this);
				return;
			}

			//Here we can recognize first difference in handle of different clients
			if (isUIServerMessage){
				Log->WriteMessage(TRACE, "Reading data from UIServer...");
				if (DataSize == 0)
				{
					Log->WriteMessage(ERR, "Nothing to be read! DataSize==0");
					sem_post(&connectionSem);
					connCount--;
					delete(this);
					return;
				}
				data.append(buf, DataSize);
				if ((data.find("</adapter_server>") != std::string::npos) || ((data[data.size() - 2] == '/') && (data[data.size() - 2] == '>')))
				{
					break;
				}
				break; //TODO Solve different way... last two chars as "/>" and then break...
			}
			else if (isAdapterServerMessage){
				Log->WriteMessage(TRACE, "Reading data from Adapter Server Reciever...");
				if (DataSize == 0)
				{
					Log->WriteMessage(ERR, "Nothing to be read! DataSize==0");
					sem_post(&connectionSem);
					connCount--;
					delete(this);
					return;
				}
				data.append(buf, DataSize);
				if ((data.find("</adapter_server>") != std::string::npos) || ((data[data.size() - 2] == '/') && (data[data.size() - 2] == '>')))
				{
					break;
				}
			}
			else if (isAlgorithmMessage){
				Log->WriteMessage(TRACE, "Reading data from Algorithm...");
				if (DataSize == 0)
				{
					Log->WriteMessage(ERR, "Nothing to be read! DataSize==0");
					sem_post(&connectionSem);
					connCount--;
					delete(this);
					return;
				}
				data.append(buf, DataSize);
				if ((data.find("</alg_m>") != std::string::npos) || ((data[data.size() - 2] == '/') && (data[data.size() - 2] == '>')))
				{
					break;
				}

			}
			else{
				Log->WriteMessage(ERR, "Something is wrong with Server port!");
				sem_post(&connectionSem);
				connCount--;
				delete(this);
				return;
			}
		}
	}
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());

	tstringXMLwriter writer;
	doc.print(writer);
	std::string acceptedMessageString = writer.result;

	if (acceptedMessageString[acceptedMessageString.length() - 1] == '\n')
	{
		acceptedMessageString.erase(acceptedMessageString.length() - 1, 1);
	}
	Log->WriteMessage(TRACE, "MESSAGE ACCEPTED: " + acceptedMessageString);

	//zaèíná parser zprávy
	if (isAdapterServerMessage){
		this->HandleAdapterMessage(data, Log, FConfig, database, &dbAccessSem);
	}
	else if (isUIServerMessage){
		this->HandleUIServerMessage(data, Log, FConfig, database, &dbAccessSem);
	}
	else if (isAlgorithmMessage) {
		this->HandleAlgorithmMessage(data, Log, FConfig, database, &dbAccessSem);
	}
	else{
		Log->WriteMessage(ERR, "Message wrong format: " + acceptedMessageString);
	}
	cont->ReturnConnection(database->GetConnectionSession());
	connCount--;
	sem_post(&connectionSem);
	delete(this);
}


/** Metoda, ktera ziska vsechny algoritmy dle adapterID  a userID a ulozi je do struktury talg , ktere vlozi postupne do vektoru a vrati.
*
* @param adapterId	id adapteru v DB
* @param userId		id uzivatele v DB
*
* @return vektor struktur talg reprezentujici jeden algoritmus 
*/
vector<talg *> FrameworkServerHandle::getAllAlgsByAdapterIdAndUserId(string adapterId, string userId){
	Log->WriteMessage(TRACE, "ENTERING FrameworkServerHandle::getAllAlgsByAdapterId");
	vector <talg *> allAlgs;
	vector<string> allIdsOfUsersAlgorithms = database->SelectAllIdsOfUsersAlgorithmsByAdapterIdAndUserId(adapterId, userId);
	for (auto it = allIdsOfUsersAlgorithms.begin(); it != allIdsOfUsersAlgorithms.end(); ++it){
		talg * tmpAlg = getAlgByUserAlgorithmId(*it);
		allAlgs.push_back(tmpAlg);
	}
	Log->WriteMessage(TRACE, "EXITING FrameworkServerHandle::getAllAlgsByAdapterId");
	return allAlgs;
}

/** Metoda, ktera ziska algoritmus dle ID uzivatelskeho algoritmu a ulozi je do struktury talg, kterou vrati
*
* @param userAlgId	id uzivatelskeho algoritmu
*
* @return struktura talg reprezentujici uzivatelsky algoritmus
*/
talg * FrameworkServerHandle::getAlgByUserAlgorithmId(string userAlgId){
	Log->WriteMessage(TRACE, "ENTERING FrameworkServerHandle::getAlgByUserAlgorithmId");
	talg * algorithmMessage = new talg();
	algorithmMessage->atype = database->SelectAlgIdByUsersAlgId(userAlgId);
	algorithmMessage->usersAlgorithmsId = userAlgId;
	algorithmMessage->enable = database->SelectStateByUsersAlgId(userAlgId);
	algorithmMessage->name = database->SelectNameByUsersAlgId(userAlgId);
	string notParsedParameters = database->SelectParametersByUsersAlgId(userAlgId);
	std::vector<std::string> childsIds = database->SelectDevIdsByUsersAlgId(userAlgId);

	//Pridani childs dev
	int posDev = 1;
	Log->WriteMessage(TRACE, "FrameworkServerHandle::getAlgByUserAlgorithmId adding devs");
	for (auto it = childsIds.begin(); it != childsIds.end(); ++it){
		tchldMsg * chldMessage = new tchldMsg();
		chldMessage->nameOfElement = "dev";
		chldMessage->id = *it;
		Log->WriteMessage(TRACE, "FrameworkServerHandle::getAlgByUserAlgorithmId adding devs, id=" + *it + ", pos=" + to_string(posDev));
		chldMessage->pos = to_string(posDev);
		chldMessage->type = database->SelectDevTypeByDevId(*it);
		algorithmMessage->childs.push_back(chldMessage);

		posDev++;
	}

	//Pridani par
	int posPar = 1;
	std::vector<std::string> parsedParams = this->parseParametersToVector(notParsedParameters);
	Log->WriteMessage(TRACE, "FrameworkServerHandle::getAlgByUserAlgorithmId adding pars");
	for (auto it = parsedParams.begin(); it != parsedParams.end(); ++it){
		tchldMsg * chldMessage = new tchldMsg();
		Log->WriteMessage(TRACE, "FrameworkServerHandle::getAlgByUserAlgorithmId adding pars, text=" + *it + ", pos=" + to_string(posPar));
		chldMessage->nameOfElement = "par";
		chldMessage->text = *it;
		chldMessage->pos = to_string(posPar);
		algorithmMessage->childs.push_back(chldMessage);

		posPar++;
	}
	Log->WriteMessage(TRACE, "EXITING FrameworkServerHandle::getAlgByUserAlgorithmId");
	return algorithmMessage;
}

vector<string> FrameworkServerHandle::parseParametersToVector(string notParsedParams){
	vector<string> parsedParams = this->explode(notParsedParams, '#');
	return parsedParams;
}

/** Pomocna metoda pro konverzi retezce na char *
*
* @param toChange	retezec ke zmene
*/
char * FrameworkServerHandle::StringToChar(string toChange){
	char *inChar = new char[toChange.length() + 1];
	strcpy(inChar, toChange.c_str());
	return inChar;
}

/** Pomocna metoda pro rozdeleni retezce dle znaku
*
* @param vektor retezcu
*/
vector<string> FrameworkServerHandle::explode(string str, char ch) {
	string next;
	vector<string> result;
	bool backslash = false;

	for (string::const_iterator it = str.begin(); it != str.end(); it++) {
		if (*it == ch && backslash == false) {
			if (!next.empty()) {
				result.push_back(next);
				next.clear();
			}
			backslash = false;
		}
		else if (*it == '\\')
		{
			backslash = true;
		}
		else {
			backslash = false;
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}

/** Metoda odesilajici zpravu na socket.
*
* @param socket			Socket, na ktery se odesle zprava
* @param xmlMessage		Text zpravy (retezec)
*
* @return pravdivostni hodnota zda byla zprava odeslana.
*/
bool FrameworkServerHandle::sendMessageToSocket(int socket, string xmlMessage){
	if (xmlMessage[xmlMessage.length() - 1] == '\n')
	{
		xmlMessage.erase(xmlMessage.length() - 1, 1);
	}
	Log->WriteMessage(TRACE, "ENTERING FrameworkServerHandle::sendMessageToSocket");
	if ((send(socket, xmlMessage.c_str(), xmlMessage.size() + 1, 0)) == -1)
	{
		Log->WriteMessage(FATAL, "sendMessageToSocket : Problem with sending message to socket - " + std::to_string(socket));
		return false;
	}
	Log->WriteMessage(INFO, "sendMessageToSocket : Data send to socket - " + std::to_string(socket));
	Log->WriteMessage(INFO, "FrameworkServerHandle::sendMessageToSocket : DATA SEND :" + xmlMessage);
	Log->WriteMessage(TRACE, "EXITING FrameworkServerHandle::sendMessageToSocket");
	return true;
}

/** Metoda odesilajici zpravu na ada server, kde je potreba jeste naslouchat pro odpoved a tuto odpoved pripadne zpracovat.
*
* @param xmlMessage		Text zpravy (retezec)
*
* @return pravdivostni hodnota zda byla zprava odeslana ci nedoslo k chybe pri prijimani odpovedi.
*/
bool FrameworkServerHandle::sendMessageToAdaServer(string xmlMessage){
	Log->WriteMessage(TRACE, "ENTERING FrameworkServerHandle::sendMessageToAdaServer");
	hostent *host;              
	sockaddr_in serverSock;     
	int socketToAdaServer;               
	int port;                   // Èíslo portu
	int size;                   // Poèet pøijatých a odeslaných bytù
	port = atoi(to_string(FConfig->portAdaSenderServer).c_str());
	if ((host = gethostbyname("localhost")) == NULL)
	{
		Log->WriteMessage(FATAL, "FrameworkServerHandle::sendMessageToAdaServer : Bad Address!");
		return false;
	}

	if ((socketToAdaServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		Log->WriteMessage(FATAL, "FrameworkServerHandle::sendMessageToAdaServer : Cannot create socket!");
		return false;
	}
	serverSock.sin_family = AF_INET;
	serverSock.sin_port = htons(port);
	memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);

	if (connect(socketToAdaServer, (sockaddr *)&serverSock, sizeof(serverSock)) == -1)
	{
		Log->WriteMessage(FATAL, "FrameworkServerHandle::sendMessageToAdaServer : Cannot get connection!");
		return false;
	}

	if ((size = send(socketToAdaServer, xmlMessage.c_str(), xmlMessage.size() + 1, 0)) == -1)
	{
		Log->WriteMessage(FATAL, "FrameworkServerHandle::sendMessageToAdaServer : Problem with sending data!");
		close(socketToAdaServer);
		return false;
	}
	Log->WriteMessage(INFO, "FrameworkServerHandle::sendMessageToAdaServer : DATA SEND TO ADA_SERVER:" + xmlMessage);
	//Answer
	ssize_t DataSize = 0;
	char buf[1000];
	int pollRes;
	struct pollfd ufds;
	ufds.fd = socketToAdaServer;
	ufds.events = POLLIN;
	std::string data;
	data.clear();

	while (1)
	{
		pollRes = poll(&ufds, 1, 1000); // 3. parametr je Timeout
		if (pollRes == -1)
		{
			Log->WriteMessage(FATAL, "FrameworkServerHandle::sendMessageToAdaServer : Problem with reading data from adaServer!");
			close(socketToAdaServer);
			return false;
		}
		if (pollRes == 0)
		{
			Log->WriteMessage(FATAL, "FrameworkServerHandle::sendMessageToAdaServer : Connection to client timed out!");
			close(socketToAdaServer);
			return false;
		}
		else
		{
			if ((DataSize = read(socketToAdaServer, &buf, 1024)) < 0) //prijmeme dlzku prichadzajucich dat (2 byty kratkeho integera bez znamienka)
			{
				Log->WriteMessage(FATAL, "FrameworkServerHandle::sendMessageToAdaServer : Problem with reading data from adaServer!");
			}

			if (DataSize == 0)
			{
				Log->WriteMessage(FATAL, "FrameworkServerHandle::sendMessageToAdaServer Nothing to be read! DataSize==0");
				break;
			}
			data.append(buf, DataSize);
		}
	}
	close(socketToAdaServer);
	Log->WriteMessage(INFO, "FrameworkServerHandle::sendMessageToAdaServer : DATA RECIEVED FROM ADA_SERVER :" + data);

	
	Log->WriteMessage(TRACE, "EXITING FrameworkServerHandle::sendMessageToAdaServer");
	return true;
}

/** Metoda parsujici parametry do Databaze
*
* @param params			struktura tparam
* @param paramsCnt		pocet parametru
*
* @return zparsovane parametry do retezce k ulozeni do DB
*/
string FrameworkServerHandle::parseParametersToDB(tparam *params, int paramsCnt){
	string toReturn = "";
	int posExp = 1;
	for (int i = 0; i < paramsCnt; i++){
		if (posExp != params[i].pos)
			Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage: parseParametersToDB something wrong");

		Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage: parseParametersToDB content of params[i].text: " + params[i].text);

		if (i == (paramsCnt - 1)){
			toReturn += params[i].text;
		}
		else{
			toReturn += params[i].text + "#";
		}
		posExp++;
	}
	return toReturn;
}

/** Metoda spoustejici program se zadanymi argumenty.
*
* @param program			nazev binarky programu
* @param arg_list			seznam parametru predanych binarce pri spusteni
*
* @return zparsovane parametry do retezce k ulozeni do DB
*/
int FrameworkServerHandle::spawn(char* programBinaryName, char** arg_list)
{
	pid_t childProcessId;
	childProcessId = fork();
	if (childProcessId != 0)
		//Rodicovsky program
		return childProcessId;
	else {
		//Program potomek - zde se vyvola program dle parametru
		execvp(programBinaryName, arg_list);

		//Tento kod se vyvola pouze pokud nastane chyba v execvp
		if (programBinaryName != nullptr){
			std::string programBinaryNameString(programBinaryName);
			Log->WriteMessage(ERR, "FrameworkServerHandle::spawn Error occured during execvp! Binary name: " + programBinaryNameString);
		}
		else{
			cerr << "FRAMEWORK ERROR: FrameworkServerHandle::spawn Error occured during execvp!" << endl;
		}
		abort();
	}
}
/** Konstruktor objektu FrameworkServerHandle. Je to objekt pro obsluhu serverovych spojeni.
*
* @param init_socket			socket na kterem je navazano spojeni a ktere se bude obsluhovat
* @param init_port				port serveru, ktery obsluhuje spojeni (potreba pro nasledne rozliseni jakou obsluhu ma vybrat)
*/
FrameworkServerHandle::FrameworkServerHandle(int init_socket, int init_port){
	this->handledSocket = init_socket;
	this->port = init_port;
}

/** Destruktor objektu FrameworkServerHandle.
*
*/
FrameworkServerHandle::~FrameworkServerHandle(){
	close(this->handledSocket);
}


//---------------------------------------  CREATING MESSAGES ---------------------------------------


//Zde jsou implementace zprav protokolu IOT - smerem k UI serveru a nasledne do mobilniho zarizeni

/** Metoda vytvarejici zpravu algcreated dle algId 
*
* @param algId		ID vytvoreneho uzivatelskeho algoritmu
*
* @return XML zprava v retezci
*/
string FrameworkServerHandle::createMessageAlgCreated(string algId){
	xml_document *resp = new xml_document();

	//Definuj element algorithm_message
	xml_node algorithm_message = resp->append_child("com");
	algorithm_message.append_attribute("ver");
	algorithm_message.append_attribute("state");
	algorithm_message.append_attribute("algid");
	//Nastav element algorithm_message
	algorithm_message.attribute("ver") = "2.5";
	algorithm_message.attribute("state") = "algcreated";
	algorithm_message.attribute("algid") = algId.c_str();

	string emptyString = " ";
	pugi::xml_node nodechild = algorithm_message.append_child(pugi::node_pcdata);
	nodechild.set_value(emptyString.c_str());

	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	std::string tmp = writer.result;
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + tmp;
}

/** Metoda vytvarejici zpravu false dle error kodu zadaneho parametrem
*
* @param errcode		https://ant-2.fit.vutbr.cz/projects/android-app/wiki/Smarthome_Phone-Server_Protocol_XML_#Info-tabulky
*
* @return XML zprava v retezci
*/
string FrameworkServerHandle::createMessageFalse(string errcode){
	xml_document *resp = new xml_document();
	//Definuj element algorithm_message
	xml_node algorithm_message = resp->append_child("com");
	algorithm_message.append_attribute("ver");
	algorithm_message.append_attribute("state");
	algorithm_message.append_attribute("errcode");
	//Nastav element algorithm_message
	algorithm_message.attribute("ver") = "2.5";
	algorithm_message.attribute("state") = "false";
	algorithm_message.attribute("errcode") = errcode.c_str(); 

	string emptyString = " ";
	pugi::xml_node nodechild = algorithm_message.append_child(pugi::node_pcdata);
	nodechild.set_value(emptyString.c_str());

	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	std::string tmp = writer.result;
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + tmp;
}

/** Metoda vytvarejici zpravu true 
*
* @return XML zprava v retezci
*/
string FrameworkServerHandle::createMessageTrue(){
	xml_document *resp = new xml_document();
	//Definuj element algorithm_message
	xml_node algorithm_message = resp->append_child("com");
	algorithm_message.append_attribute("ver");
	algorithm_message.append_attribute("state");
	//Nastav element algorithm_message
	algorithm_message.attribute("ver") = "2.5";
	algorithm_message.attribute("state") = "true";

	string emptyString = " ";
	pugi::xml_node nodechild = algorithm_message.append_child(pugi::node_pcdata);
	nodechild.set_value(emptyString.c_str());

	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	std::string tmp = writer.result;
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + tmp;
}

/** Metoda vytvarejici zpravu algs
*
* @param allAlgs		vektor struktur talg reprezentujici seznam uzivatelskych algoritmu
* @param adapterId		Id adapteru
*
* @return XML zprava v retezci
*/
string FrameworkServerHandle::createMessageAlgs(vector<talg *> allAlgs, string adapterId){

	xml_document *resp = new xml_document();

	xml_node algorithm_message = resp->append_child("com");
	algorithm_message.append_attribute("ver");
	algorithm_message.append_attribute("state");
	algorithm_message.append_attribute("aid");

	algorithm_message.attribute("ver") = "2.5";
	algorithm_message.attribute("state") = "algs";
	algorithm_message.attribute("aid") = adapterId.c_str();

	string emptyString = " ";
	pugi::xml_node nodechild = algorithm_message.append_child(pugi::node_pcdata);
	nodechild.set_value(emptyString.c_str());

	//Add algorithms into message
	for (auto it = allAlgs.begin(); it != allAlgs.end(); ++it){
		xml_node alg = algorithm_message.append_child("alg"); // <alg ...
		alg.append_attribute("id");
		alg.append_attribute("atype");
		alg.append_attribute("enable");
		alg.append_attribute("name");

		alg.attribute("id") = ((*it)->usersAlgorithmsId).c_str();
		alg.attribute("atype") = ((*it)->atype).c_str();
		alg.attribute("enable") = ((*it)->enable).c_str();
		alg.attribute("name") = ((*it)->name).c_str();

		std::vector<tchldMsg*> childs = (*it)->childs;
		for (auto it2 = childs.begin(); it2 != childs.end(); ++it2){
			string nameOfCurrElement = (*it2)->nameOfElement;
			xml_node child;

			if (nameOfCurrElement.compare("par") == 0){ // <par ...
				//par
				child = alg.append_child("par");
				child.append_attribute("pos");
				pugi::xml_node nodechild = child.append_child(pugi::node_pcdata);
				child.attribute("pos") = ((*it2)->pos).c_str();
				nodechild.set_value(((*it2)->text).c_str());
			}
			else if (nameOfCurrElement.compare("dev") == 0){ //<dev ...
				//dev
				child = alg.append_child("dev");
				child.append_attribute("id");
				child.append_attribute("type");
				child.append_attribute("pos");
				child.attribute("id") = ((*it2)->id).c_str();
				child.attribute("type") = ((*it2)->type).c_str();
				child.attribute("pos") = ((*it2)->pos).c_str();
			}
			else{
				//budouci, napr geo
			}
		}
	}

	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	std::string tmp = writer.result;
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + tmp;
}

//Zde jsou implementace zprav smerem na Adapter Server Sender

/** Metoda vytvarejici zpravu o zmìnì aktoru z jednoho stavu na stav druhý.
*
* @param id				id aktporu
* @param type			typ aktoru
* @param adapterId		Id adapteru
*
* @todo Zadny z oddeleni senzoru nebyl schopen zatim specifikovat presny tvar zpravy. Momentalne zprava pouze prepina z vypnuto na zapnuto. 
*
* @return XML zprava v retezci
*/
string FrameworkServerHandle::createMessageRequestSwitch(string id, string type, string adapterId){
	/*
	<request type="switch">
    <sensor id="1.1.1.1" type="0x00" onAdapter=12345">
        <value>ON</value>
    </sensor>
	</request>
	*/
	int typeInt = strtol(type.c_str(), NULL, 10);
	std::ostringstream os;
	os << std::hex << typeInt;
	std::string hexaType = "0x" + os.str();

	xml_document *resp = new xml_document();

	xml_node request_message = resp->append_child("request");
	request_message.append_attribute("type");
	request_message.attribute("type") = "switch";

	pugi::xml_node sensor_node = request_message.append_child("senzor");
	sensor_node.append_attribute("id");
	sensor_node.append_attribute("type");
	sensor_node.append_attribute("onAdapter");
	sensor_node.attribute("id") = id.c_str();
	sensor_node.attribute("type") = hexaType.c_str();
	sensor_node.attribute("onAdapter") = adapterId.c_str();

	pugi::xml_node sensor_value_node = sensor_node.append_child("value");
	pugi::xml_node sensor_value_node_pcdata = sensor_value_node.append_child(pugi::node_pcdata);
	sensor_value_node_pcdata.set_value("ON");

	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	std::string tmp = writer.result;
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + tmp;
}

/** Funkce pro obsluhu prijatych signalu procesu
*
* @param signo		cislo prijateho signalu
*/
void sig_handler(int signo)
{
	if (signo == SIGTERM)
	{
		delete(UIServer);
		delete(AdaServer);
		delete(AlgServer);
		delete(cont);
		sem_destroy(&connectionSem);
		sem_destroy(&dbAccessSem);
		delete(Log);
		delete(FConfig);
		exit(EXIT_SUCCESS);
	}
	if (signo == SIGINT){
		FConfig->ResetAlgorithms();
		//Nastaveni seznamu algoritmu do tabulky u_algorithms do databaze
		//Ziskani spojení s DB
		session *Conn = NULL;
		DBFWHandler * database = NULL;
		sem_wait(&connectionSem);
		connCount++;

		Conn = cont->GetConnection();
		if (Conn == NULL)
		{
			Log->WriteMessage(INFO, "ALL CONNECTIONS ARE USED!");
		}
		try
		{
			database = new DBFWHandler(Conn, Log);
		}
		catch (std::exception &e)
		{
			Log->WriteMessage(ERR, "Unable to create DBHandler during reset of Application!");
		}
		FConfig->SetUpAlgorithmsInDatabase(database);
		cont->ReturnConnection(database->GetConnectionSession());
		sem_post(&connectionSem);
		delete(database);
	}
}

//---------------------------------------  MAIN ---------------------------------------

int main()
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
		std::cerr << "Unable to create memory space for Loger, exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}
	try{
		FConfig = new FrameworkConfig();
	}
	catch (std::exception &e){
		std::cerr << e.what() << std::endl;
		std::cerr << "Unable to create memory space for FrameworkConfig, exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}
	//Nastaveni configu
	FConfig->SetConfig("config.xml");

	connCount = 0;
	//Nastaveni kontejneru pro DB
	cont = DBConnectionsContainer::GetConnectionContainer(Log, FConfig->dbName, FConfig->maxNumberDBConnections);
	if (cont == nullptr){
		std::cerr << "Unable to create memory space for DBConnectionsContainer, exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}

	//Nastaveni semaforu pro omezeni poctu pripojeni k DB
	int semVal = FConfig->maxNumberDBConnections;
	Log->WriteMessage(FATAL, "Maximal conn count : " + std::to_string(semVal));
	sem_init(&connectionSem, 0, semVal);
	//Nastaveni semaforu pro vylucny pristup prace s databazi
	sem_init(&dbAccessSem, 0, 1);
	//Nastaveni Loggeru
	Log->SetLogger(	FConfig->loggerSettingVerbosity, 
					FConfig->loggerSettingFilesCnt,
					FConfig->loggerSettingLinesCnt,
					FConfig->loggerSettingFileName,
					".",
					FConfig->loggerSettingAppName);

	FConfig->SetLogger(Log);

	//Nastaveni seznamu algoritmu do tabulky u_algorithms do databaze
	//Ziskani spojení s DB
	session *Conn = NULL;
	Conn = cont->GetConnection();
	DBFWHandler * database = NULL;
	try
	{
		database = new DBFWHandler(Conn, Log);
	}
	catch (std::exception &e)
	{
		Log->WriteMessage(ERR, "Unable to create DBHandler during inicialization of Application!");
	}
	FConfig->SetUpAlgorithmsInDatabase(database);
	cont->ReturnConnection(database->GetConnectionSession());
	delete(database);
	//Instanciace serveru prijimajici zpravy od UI Serveru
	try
	{
		UIServer = new FrameworkServer(FConfig->portUIServer);
		std::thread worker(&FrameworkServer::StartServer, UIServer);
		worker.detach();
	}
	catch (std::exception &e)
	{
		std::cerr << "Framework: Unable to create memory space for FrameworkServer. Exiting!" << std::endl;
		delete(UIServer);
		exit(EXIT_FAILURE);
	}

	//Instanciace serveru prijimajici zpravy od Ada Serveru
	try
	{
		AdaServer = new FrameworkServer(FConfig->portAdaRecieverServer);
		std::thread worker(&FrameworkServer::StartServer, AdaServer);
		worker.detach();
	}
	catch (std::exception &e)
	{
		std::cerr << "Framework: Unable to create memory space for FrameworkServer. Exiting!" << std::endl;
		delete(AdaServer);
		exit(EXIT_FAILURE);
	}

	//Instanciace serveru prijimajici zpravy od Algoritmu (nikoliv jiz vlakno)
	try
	{
		AlgServer = new FrameworkServer(FConfig->portAlgorithmServer);
		AlgServer->StartServer();
	}
	catch (std::exception &e)
	{
		std::cerr << "Framework: Unable to create memory space for FrameworkServer. Exiting!" << std::endl;
		delete(AlgServer);
		exit(EXIT_FAILURE);
	}

	delete(UIServer);
	delete(AdaServer);
	delete(AlgServer);
	delete(cont);
	sem_destroy(&connectionSem);
	sem_destroy(&dbAccessSem);
	delete(Log);
	delete(FConfig);

	return EXIT_SUCCESS;
}



