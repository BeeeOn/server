/**
* @file framework.cpp
*
* @Implementace modulárního prostøedí pro algoritmy
*
* @author xrasov01
* @version 1.0
*/

#include "framework.h"

#define UI_FRAMEWORK_PORT 7082 
#define ADA_FRAMEWORK_PORT 7083 
#define ALG_FRAMEWORK_PORT 7084
#define BUFSIZE 1000
#define ADA_SERVER_PORT 7383
#define UI_SERVER_PORT 1111

using namespace std;
using namespace soci;
using namespace pugi;

namespace sc = std::chrono;

FrameworkServer * UIServer;					//Ukazatel na instanci serveru Frameworku
FrameworkServer * AdaServer;
FrameworkServer * AlgServer;
Loger *Log;									//Loger pro logování do souboru
DatabaseConnectionContainer *cont = NULL;	//Container pro DB
sem_t connectionSem;						//Semafor pro maximální poèet pøipojení k Frameworku
int notifIndex = 0;
std::atomic<unsigned long> connCount;

void FrameworkServer::StartServer(){
	this->SetUpSockets();
	this->AcceptConnection();
}

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
			cerr << "Chyba alokace " << endl;
			close(newSocket);
		}
		catch (std::exception &e)
		{
			cerr << "Client won't be served unable to crate thread! " << endl;
			close(newSocket);
		}
	}
	return 0;
}

FrameworkServer::FrameworkServer(int init_port){
	this->port = init_port;
}

FrameworkServer::~FrameworkServer(){
	close(this->serverSocket);
}

//FrameworkServerHandle

void FrameworkServerHandle::HandleClientConnection(){
	Log->WriteMessage(ERR, "Entering HandleClientConnection");
	bool isAdapterServerMessage = false;
	bool isUIServerMessage = false;
	bool isAlgorithmMessage = false;

	switch (this->port) {
		case UI_FRAMEWORK_PORT:
			isUIServerMessage = true;
			break;

		case ADA_FRAMEWORK_PORT:
			isAdapterServerMessage = true;
			break;

		case ALG_FRAMEWORK_PORT:
			isAlgorithmMessage = true;
			break;

		default:
			Log->WriteMessage(ERR, "Something is wrong with Server port!");
			return;
	}

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
		connCount--;
		delete (this);
		return;
	}

	char buf[BUFSIZE];				// Pøijímací buffer
	//int size;						// Poèet pøijatých a odeslaných bytù
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
			if ((DataSize = read(this->handledSocket, &buf, 1024)) < 0) //prijmeme dlzku prichadzajucich dat (2 byty kratkeho integera bez znamienka)
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
		Log->WriteMessage(TRACE, "HandleClientConnection: Entering AdapterServerMessage");
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

		//----------------------Zde testovat obsah zpravy a na zaklade neho mathovat s db a pokud ano pak spustit Algoritmus-------------------

		//Nejdrive match zda existuje zaznam v UserAlgorithm s algoritmem na dany adapter
		//  pokud ano, pak vzit UserAlgId s timto adapterem a testovat na mac senzoru
		//Pokud se alespon jeden z tech senzoru matchne => vyvolat algoritmus
		xml_node adapterMessage = doc.child("adapter_server");
		string adapterId = adapterMessage.attribute("adapter_id").value();

		xml_node dev = adapterMessage.child("device");
		string deviceId = dev.attribute("id").value();
		int deviceIdInt = strtol(deviceId.c_str(), NULL, 0);

		string adapterIDString = to_string(this->parsedMessage->adapterINTid);
		Log->WriteMessage(TRACE, "adapterIDString = " + adapterIDString);
		vector<string> allIdsOfUsersAlgorithms = database->SelectIdsEnabledAlgorithmsByAdapterId(adapterIDString);

		if ( ! allIdsOfUsersAlgorithms.empty() ){
			Log->WriteMessage(TRACE, "HandleClientConnection: AdapterServerMessage - after condition(!allIdsOfUsersAlgorithms.empty()) && parsedMessage->state != 0 ");
			//Nalezen alespon jeden UserAlg s danym AdapterId
			for (auto idOfUsersAlgorithms = allIdsOfUsersAlgorithms.begin(); idOfUsersAlgorithms != allIdsOfUsersAlgorithms.end(); ++idOfUsersAlgorithms){
				vector<string> allSenzorIds = database->SelectDevIdsByUsersAlgId(*idOfUsersAlgorithms);
				if (! allSenzorIds.empty()){
					Log->WriteMessage(TRACE, "HandleClientConnection: AdapterServerMessage - after SECOND condition (! allSenzorIds.empty())");
					//Nasel mac pro UserAlg
					for (auto itSenzorId = allSenzorIds.begin(); itSenzorId != allSenzorIds.end(); ++itSenzorId){
						int currentDevId = strtol((*itSenzorId).c_str(), NULL, 10);
						Log->WriteMessage(TRACE, "HandleClientConnection: AdapterServerMessage - THIRD THESE TWO - currentDevId:" + to_string(currentDevId) + ", deviceIdInt: " + to_string(deviceIdInt));

						if (currentDevId == deviceIdInt){
							for (int i = 0; i < this->parsedMessage->values_count; i++)
							{
								//if (this->parsedMessage->values[i].type != TEMP)

								//Start algorithm WATCHDOG!!!
								//  The argument list to pass to the Algorithm
								string UserId = database->SelectUserIdByUsersAlgId(*idOfUsersAlgorithms);
								string AlgId = database->SelectAlgIdByUsersAlgId(*idOfUsersAlgorithms);
								string parametersTmp = database->SelectParametersByUsersAlgId(*idOfUsersAlgorithms);
								string parameters = '"' + parametersTmp + '"';

								//Prepare senzor values for argument -v
								string senzorValues = "";
								senzorValues += "ID=" + (*itSenzorId) + "#";
								senzorValues += "type=" + to_string(this->parsedMessage->values[i].type) + "#";
								if (this->parsedMessage->values[i].type == TEMP || this->parsedMessage->values[i].type == HUM ){ //Predavani do fval

									senzorValues += "fval=" + to_string(this->parsedMessage->values[i].fval) + "#";
								}
								else { //Predavani do fval

									senzorValues += "ival=" + to_string(this->parsedMessage->values[i].fval) + "#";
								}

								senzorValues += "offset=" + to_string(this->parsedMessage->values[i].offset);						
								string nameOfBinary = "watch_and_notify";

								Log->WriteMessage(INFO, "EXECUTING ALGORITHM BINARY " + nameOfBinary + " - AlgId: " + AlgId + " , userId: " + UserId + ", parameters: " + parametersTmp + ", senzorValues: " + senzorValues);

								char* arg_list[] = {
									StringToChar(nameOfBinary),
									StringToChar("-u"), // userID
									StringToChar(UserId),
									StringToChar("-a"), // algID
									StringToChar(AlgId),
									StringToChar("-d"), // adapterID
									StringToChar(adapterIDString),
									StringToChar("-o"), // UsersAlgId
									StringToChar(*idOfUsersAlgorithms),
									StringToChar("-v"), //Senzor Values
									StringToChar(senzorValues),
									StringToChar("-p"), // parameters given by User
									StringToChar(parametersTmp),
									StringToChar("/"),
									NULL
								};
								this->spawn(StringToChar("Algorithms/watch_and_notify"), arg_list);

								Log->WriteMessage(INFO, "EXECUTED ALGORITHM BINARY " + nameOfBinary + "- AlgId: " + AlgId + "userId: " + UserId + ", parameters: " + parametersTmp + ", senzorValues: " + senzorValues);
							}
						}
					}
				}
			}
		}
		//---------------------------------------------------------------------------------------------





	}
	else if (isUIServerMessage){
		Log->WriteMessage(TRACE, "HandleClientConnection: Entering UiServerMessage");
		//Kód zpracující zprávy od UIServeru (Pøeposílání zpráv od uživatelù Androidu)
		xml_node algMessage = doc.child("com");

		string ver = algMessage.attribute("ver").value();
		string state = algMessage.attribute("state").value();
		string bt = algMessage.attribute("bt").value();
		string userId = algMessage.attribute("userid").value();
		
		bool error = false;
		string stringToSendAsAnswer = "";
		
		if (state.compare("addalg") == 0){
			//zpracování zprávy addalg
			string adapterId = algMessage.attribute("aid").value();
			string algId = algMessage.attribute("atype").value();
			string algName = algMessage.attribute("algname").value();
			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : UIServerMessageone addalg");		
			//Zde z configu musim ziskat informace o algoritmu kolik bude mit parametru a devices
			//Momentalne jen max 2 device a 5 parametru - WATCHDOG 
			int maxdevsToExpect = 2;
			int maxparamsToExpect = 5;

			//inicializace poli do kterych se budou predavat struktury uchovavajici dev a params
			tdevice * devices = new tdevice[maxdevsToExpect];
			tparam * params = new tparam[maxparamsToExpect];

			//Zpracovat dev
			xml_node child = algMessage.first_child();
			int numberOfDevs = 0;
			for (pugi::xml_node algNode = algMessage.first_child(); algNode; algNode = algNode.next_sibling())
			{
				string name = algNode.name();
				if (name.compare("dev") == 0){
					int devId = child.attribute("id").as_int();
					int devType = child.attribute("type").as_int();
					int devPos = child.attribute("pos").as_int();
					devices[numberOfDevs].id = devId;
					devices[numberOfDevs].type = devType;
					devices[numberOfDevs].pos = devPos;
					numberOfDevs++;
				}
			}

			//Zpracovani par
			child = algMessage.first_child();
			int numberOfParams = 0;
			for (pugi::xml_node algNode = algMessage.first_child(); algNode; algNode = algNode.next_sibling())
			{
				string name = algNode.name();
				if (name.compare("par") == 0){
					int parPos = algNode.attribute("pos").as_int();
					string parText = algNode.child_value();
					params[numberOfParams].pos = parPos;
					params[numberOfParams].text = parText;
					numberOfParams++;
				}
			}

			if (numberOfDevs > maxdevsToExpect || numberOfParams > maxparamsToExpect){
				Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage : incorrect message according to specification of algorithm!");
				sem_post(&connectionSem);
				connCount--;
				stringToSendAsAnswer = this->createMessageFalse("12");
				//ODESLANI ODPOVEDI NA UI SERVER - TEDY NA ANDROID ZARIZENI
				if (!this->sendMessageToSocket(this->handledSocket, stringToSendAsAnswer)){
					//Error sendind data
				}
				delete(this);
				return;
			}

			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : preParsedParameters");
			//Predzpracovat string parametru
			string preParsedParameters = parseParametersToDB(params, numberOfParams);

			string newUserALgorithmId = database->InsertUserAlgorithm(userId, algId, preParsedParameters, algName, adapterId);

			if (newUserALgorithmId == "0"){
				Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage : Adding UserAlgorithm to DB failure!");
				error = true;
			}
			else{
				Log->WriteMessage(INFO, "HandleClientConnection: UIServerMessage : NewUserAlgorithmId - " + newUserALgorithmId);
			}
			for (int i = 0; i < numberOfDevs; i++){
				if (!database->InsertAlgoDevices(newUserALgorithmId, userId, algId, std::to_string(devices[i].id), std::to_string(devices[i].type))){
					Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage : Adding AlgoDevices to DB failure!");
					error = true;
				}
			}	

			//Odeslat nazpet zpravu ohledne uspesnem pridani algoritmu
			if (! error){
				stringToSendAsAnswer = this->createMessageAlgCreated(newUserALgorithmId);
			}
			else{
				stringToSendAsAnswer = this->createMessageFalse("12");
			}


		}
		else if (state.compare("getallalgs") == 0){
			//zpracování zprávy getallalgs
			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : getAllAlgs ENTERING");

			string ver = algMessage.attribute("ver").value();
			string state = algMessage.attribute("state").value();
			string adapterId = algMessage.attribute("aid").value();
			
			vector<talg *> allAlgs = getAllAlgsByAdapterIdAndUserId(adapterId, userId);
			if (!error){
				stringToSendAsAnswer = this->createMessageAlgs(allAlgs, adapterId);
			}
			else{
				stringToSendAsAnswer = this->createMessageFalse("12");
			}

		}
		else if (state.compare("getalgs") == 0){
			//zpracování zprávy getalgs
			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : getalgs ENTERING");
			vector<talg *> allAlgs;
			string adapterId = algMessage.attribute("aid").value();

			for (pugi::xml_node algNode = algMessage.first_child(); algNode; algNode = algNode.next_sibling())
			{
				string userAlgId = algNode.attribute("id").as_string();
				allAlgs.push_back(this->getAlgByUserAlgorithmId(userAlgId));
			}
			if (!error){
				stringToSendAsAnswer = this->createMessageAlgs(allAlgs, adapterId);
			}
			else{
				stringToSendAsAnswer = this->createMessageFalse("12");
			}
		}
		else if (state.compare("setalg") == 0){
			//zpracování zprávy setalg
			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : setalg ENTERING");
			string userAlgId = algMessage.attribute("algid").value();
			string enable = algMessage.attribute("enable").value();
			string algId = algMessage.attribute("atype").value();
			string algName = algMessage.attribute("algname").value();
			//Zpracovat prijate parametry do stringu
			//Zde z configu musim ziskat informace o algoritmu kolik bude mit parametru a devices
			//Momentalne jen 1 device a 4 parametry - WATCHDOG
			//Zde z configu musim ziskat informace o algoritmu kolik bude mit parametru a devices
			//Momentalne jen max 2 device a 5 parametru - WATCHDOG 
			int maxdevsToExpect = 2;
			int maxparamsToExpect = 5;

			//inicializace poli do kterych se budou predavat struktury uchovavajici dev a params
			tdevice * devices = new tdevice[maxdevsToExpect];
			tparam * params = new tparam[maxparamsToExpect];

			//Zpracovat dev
			xml_node child = algMessage.first_child();
			int numberOfDevs = 0;
			for (pugi::xml_node algNode = algMessage.first_child(); algNode; algNode = algNode.next_sibling())
			{
				string name = algNode.name();
				if (name.compare("dev") == 0){
					int devId = child.attribute("id").as_int();
					int devType = child.attribute("type").as_int();
					int devPos = child.attribute("pos").as_int();
					devices[numberOfDevs].id = devId;
					devices[numberOfDevs].type = devType;
					devices[numberOfDevs].pos = devPos;
					numberOfDevs++;
				}
			}

			//Zpracovani par
			child = algMessage.first_child();
			int numberOfParams = 0;
			for (pugi::xml_node algNode = algMessage.first_child(); algNode; algNode = algNode.next_sibling())
			{
				string name = algNode.name();
				if (name.compare("par") == 0){
					int parPos = algNode.attribute("pos").as_int();
					string parText = algNode.child_value();
					params[numberOfParams].pos = parPos;
					params[numberOfParams].text = parText;
					numberOfParams++;
				}	
			}

			if (numberOfDevs > maxdevsToExpect || numberOfParams > maxparamsToExpect){
				Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage : incorrect message according to specification of algorithm!");
				sem_post(&connectionSem);
				connCount--;
				stringToSendAsAnswer = this->createMessageFalse("12");
				//ODESLANI ODPOVEDI NA UI SERVER - TEDY NA ANDROID ZARIZENI
				if (!this->sendMessageToSocket(this->handledSocket, stringToSendAsAnswer)){
					//Error sendind data
				}
				delete(this);
				return;
			}
			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : preParsedParameters");
			//Predzpracovat string parametru
			string preParsedParameters = parseParametersToDB(params, numberOfParams);
			//Delete devs
			if (!database->DeleteAlgoDevices(userAlgId)){
				error = true;
			}
			//Add new devs
			for (int i = 0; i < numberOfDevs; i++){
				if (!database->InsertAlgoDevices(userAlgId, userId, algId, std::to_string(devices[i].id), std::to_string(devices[i].type))){
					Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage : Adding AlgoDevices to DB failure!");
					error = true;
				}
			}
			//Update UserAlg
			if (!database->UpdateUserAlgorithm(userAlgId, algId, preParsedParameters, algName, enable)){
				error = true;
			}

			if (!error){
				stringToSendAsAnswer = this->createMessageTrue();
			}
			else{
				stringToSendAsAnswer = this->createMessageFalse("12");
			}

		}
		else if (state.compare("delalg") == 0){
			//zpracování zprávy getalgs
			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : delalg ENTERING");
			string userAlgId = algMessage.attribute("algid").value();
			//Delete devs
			if (!database->DeleteAlgoDevices(userAlgId)){
				error = true;
			}
			//Delete userAlg
			if (!database->DeleteUsersAlgorithms(userAlgId)){
				error = true;
			}

			if (!error){
				stringToSendAsAnswer = this->createMessageTrue();
			}
			else{
				stringToSendAsAnswer = this->createMessageFalse("12");
			}
		}
		else {
			//ostatni typy zprav

		}


		//ODESLANI ODPOVEDI NA UI SERVER - TEDY NA ANDROID ZARIZENI
		if (!this->sendMessageToSocket(this->handledSocket, stringToSendAsAnswer)){
			//Error sendind data
		}

	}
	else if (isAlgorithmMessage) {
		//Kód zpracující zprávy od Algoritmù
		xml_node algMessage = doc.child("alg_m");

		string userID = algMessage.attribute("userID").value();
		string algID = algMessage.attribute("algID").value();
		string adapterID = algMessage.attribute("adapterID").value();
		string offset = algMessage.attribute("offset").value();
		//float PV = algMessage.attribute("protocol_version").as_float();

		int adapterIdInt = std::stoi(adapterID);


		xml_node notifications = algMessage.child("notifs");
		int notifsCnt = notifications.attribute("count").as_uint();
		xml_node notification = notifications.first_child();
		//tnotify * notifyArray = new tnotify[notifsCnt];

		int notifsIndex = notifsCnt - 1;
		if (notifsIndex >= 0){
			auto time = sc::system_clock::now(); 
			auto since_epoch = time.time_since_epoch(); 
			auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
			long now = millis.count();

			std::vector<std::string> *IDs;
			if ((IDs = database->GetNotifStringByUserId(userID)) != nullptr){
				for (int i = 0; i <= notifsIndex; i++){
					//int notifyType = notification.attribute("type").as_int();
					string notifyText = notification.attribute("text").value();
					Notification *notif = new LimitExceededNotification(userID, notifIndex++, *IDs, now, notifyText, adapterIdInt, "111", 1, 2);
					Notificator::sendNotification(*notif);
					delete notif;
					//Saving notification to field for future purposes
					//notifyArray[i].notifyType = notifyType;
					//notifyArray[i].notifyText = notifyText;
					//std::cout << "notifyText = " << notification.attribute("text").value() << std::endl;
					notification = notification.next_sibling();
				}
				delete IDs;
			}
			else{
				Log->WriteMessage(FATAL, "HandleClientConnection: AlgorithmMessage: No GCMid for UserId");
			}
		}

		xml_node togleActors = algMessage.child("tactors");
		int togglesCnt = togleActors.attribute("count").as_uint();
		xml_node toggle = togleActors.first_child();
		//tnotify * notifyArray = new tnotify[notifsCnt];

		int toggleIndex = togglesCnt - 1;
		if (toggleIndex >= 0){

			for (int i = 0; i <= toggleIndex; i++){
				string idOfActor = toggle.attribute("id").value();
				string typeOfActor = toggle.attribute("type").value();
				//Odeslat zpravu na adapter server
				string messageToAdaServer = this->createMessageRequestSwitch(idOfActor, typeOfActor, adapterID);
				this->sendMessageToAdaServer(messageToAdaServer);
				toggle = toggle.next_sibling();
			}

		}

	}
	else{
		//Error
	}
	cont->ReturnConnection(database->ReturnConnection());
	sem_post(&connectionSem);
	connCount--;
	delete(this);
	return;
}

char * FrameworkServerHandle::StringToChar(string toChange){
	char *inChar = new char[toChange.length() + 1];
	strcpy(inChar, toChange.c_str());
	return inChar;
}


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

vector<string> FrameworkServerHandle::explode(string str, char ch) {
	string next;
	vector<string> result;
	bool backslash = false;

	// For each character in the string
	for (string::const_iterator it = str.begin(); it != str.end(); it++) {
		// If we've hit the terminal character
		if (*it == ch && backslash == false) {
			// If we have some characters accumulated
			if (!next.empty()) {
				// Add them to the result vector
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
			// Accumulate the next character into the sequence
			backslash = false;
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}

bool FrameworkServerHandle::sendMessageToSocket(int socket, string xmlMessage){
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

bool FrameworkServerHandle::sendMessageToAdaServer(string xmlMessage){
	Log->WriteMessage(TRACE, "ENTERING FrameworkServerHandle::sendMessageToAdaServer");
	hostent *host;              
	sockaddr_in serverSock;     
	int socketToAdaServer;               
	int port;                   // Èíslo portu
	int size;                   // Poèet pøijatých a odeslaných bytù
	port = atoi("7081");
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

/**********************************************Creating messages for UI server**********************************************************/


string FrameworkServerHandle::createMessageAlgCreated(string algId){
	xml_document *resp = new xml_document();

	//Definuj element algorithm_message
	xml_node algorithm_message = resp->append_child("com");
	algorithm_message.append_attribute("ver");
	algorithm_message.append_attribute("state");
	algorithm_message.append_attribute("algid");
	//Nastav element algorithm_message
	algorithm_message.attribute("ver") = "2.4";
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
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + tmp;
}

string FrameworkServerHandle::createMessageFalse(string errcode){
	// https://ant-2.fit.vutbr.cz/projects/android-app/wiki/Smarthome_Phone-Server_Protocol_XML_#Info-tabulky
	xml_document *resp = new xml_document();
	//Definuj element algorithm_message
	xml_node algorithm_message = resp->append_child("com");
	algorithm_message.append_attribute("ver");
	algorithm_message.append_attribute("state");
	algorithm_message.append_attribute("errcode");
	//Nastav element algorithm_message
	algorithm_message.attribute("ver") = "2.4";
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
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + tmp;
}

string FrameworkServerHandle::createMessageTrue(){
	xml_document *resp = new xml_document();
	//Definuj element algorithm_message
	xml_node algorithm_message = resp->append_child("com");
	algorithm_message.append_attribute("ver");
	algorithm_message.append_attribute("state");
	//Nastav element algorithm_message
	algorithm_message.attribute("ver") = "2.4";
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
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + tmp;
}

string FrameworkServerHandle::createMessageAlgs(vector<talg *> allAlgs, string adapterId){

	xml_document *resp = new xml_document();

	xml_node algorithm_message = resp->append_child("com");
	algorithm_message.append_attribute("ver");
	algorithm_message.append_attribute("state");
	algorithm_message.append_attribute("aid");

	algorithm_message.attribute("ver") = "2.4";
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
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" + tmp;
}

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

FrameworkServerHandle::FrameworkServerHandle(int init_socket, int init_port){
	this->handledSocket = init_socket;
	this->port = init_port;
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
		delete(UIServer);
		delete(AdaServer);
		delete(AlgServer);
		delete(cont);
		sem_destroy(&connectionSem);
		delete(Log);
	}
	exit(EXIT_SUCCESS);
}


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
		std::cerr << "Unable to create memory space for logging exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}

	connCount = 0;
	//Nastaveni kontejneru pro DB
	cont = DatabaseConnectionContainer::CreateContainer(Log, "home5", 30);
	//Nastaveni semaforu pro omezeni poctu pripojeni k DB
	int semVal = 30;
	Log->WriteMessage(FATAL, "Maximal conn count : " + std::to_string(semVal));
	sem_init(&connectionSem, 0, semVal);
	//Nastaveni Loggeru
	Log->SetLogger(7, 5, 100, "framework_log", "FRAMEWORK");

	//Instanciace serveru prijimajici zpravy od UI Serveru
	try
	{
		UIServer = new FrameworkServer(UI_FRAMEWORK_PORT);
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
		AdaServer = new FrameworkServer(ADA_FRAMEWORK_PORT);
		std::thread worker(&FrameworkServer::StartServer, AdaServer);
		worker.detach();
	}
	catch (std::exception &e)
	{
		std::cerr << "Framework: Unable to create memory space for FrameworkServer. Exiting!" << std::endl;
		delete(UIServer);
		exit(EXIT_FAILURE);
	}

	//Instanciace serveru prijimajici zpravy od Algoritmu (nikoliv jiz vlakno)
	try
	{
		AlgServer = new FrameworkServer(ALG_FRAMEWORK_PORT);
		AlgServer->StartServer();
	}
	catch (std::exception &e)
	{
		std::cerr << "Framework: Unable to create memory space for FrameworkServer. Exiting!" << std::endl;
		delete(UIServer);
		exit(EXIT_FAILURE);
	}

	delete(UIServer);
	delete(AdaServer);
	delete(AlgServer);

	return 0;
}



