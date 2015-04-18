/**
* @file algorithm.cpp
*
* @Implementace metod pro tvorbu algoritmù.
*
* @author xrasov01
* @version 1.0
*/
#include "algorithm.h"

using namespace std;
using namespace pugi;
using namespace soci;

#define FW_PORT "7084"

// Konstruktor Algoritmu, bude pøedán do algoritmu pod názvem instance alg
Algorithm::Algorithm(std::string init_userID, std::string init_algID, std::string init_adapterID,
	std::string init_offset, multimap<unsigned int, map<string, string>> init_values, std::vector<std::string> init_parameters, vector<tRidValues *> init_Rids){

	this->userID = init_userID;
	this->algID = init_algID;
	this->adapterID = init_adapterID;
	this->offset = init_offset;
	this->values = init_values; 
	this->parameters = init_parameters;
	this->Rids = init_Rids;
	try
	{
		this->Log = new Loger();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "Unable to create memory space for logging exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}
	//Nastaveni kontejneru pro DB
	this->cont = DatabaseConnectionContainer::CreateContainer(Log, "home5", 1);
	this->Log->SetLogger(7, 5, 100, "algorithm_log",".", "ALGORITHM");
	session *Conn = NULL;
	while (Conn == NULL)
	{
		Conn = cont->GetConnection();
		if (Conn == NULL)
		{
			this->Log->WriteMessage(INFO, "ALL CONNECTIONS ARE USED!");
		}
	}
	try
	{
		this->database = new DBHandler(Conn, Log);
	}
	catch (std::exception &e)
	{

	}

}

Algorithm::~Algorithm(){
	
}

// Metoda pøidávající notifikaci uživateli algoritmu
bool Algorithm::AddNotify(unsigned short int type, std::string text){
	tnotify * newNotification;
	try
	{
		newNotification = new tnotify();
	}
	catch (std::exception &e)
	{
		newNotification = NULL;
		cerr << "Algorithm failure: Unable to allocate memory for structure tnotify!";
		return false;
	}
	newNotification->notifyType = type;
	newNotification->notifyText = text;
	this->toNotify.push_back(newNotification);

	return true;
}

// Metoda pøidávající notifikaci uživateli algoritmu
bool Algorithm::ChangeActor(std::string id, std::string type){
	
	ttoggle * newToggle;
	try
	{
		newToggle = new ttoggle();
	}
	catch (std::exception &e)
	{
		newToggle = NULL;
		cerr << "Algorithm failure: Unable to allocate memory for structure ttoggle!";
		return false;
	}
	newToggle->id = id;
	newToggle->type = type;
	this->toToggleActor.push_back(newToggle);

	return true;
}
/*
bool Algorithm::AddDataToDB(tvalue *AddValue, unsigned short int offsetInDB){

	return true;
}
*/

//Funkce provede spojení se serverem Frameworku a odešle zprávu.
bool Algorithm::SendAndExit(){
	
	string ParsedMessage = this->CreateMessage();
	hostent *host;              
	sockaddr_in serverSock;     
	int mySocket;               
	int port;                   
	int size;                   
	port = atoi(FW_PORT);
	host = gethostbyname("localhost");
	
	if ((mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		cerr << "Nelze vytvoøit soket" << endl;
		return -1;
	}
	serverSock.sin_family = AF_INET;
	serverSock.sin_port = htons(port);
	memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);

	if (connect(mySocket, (sockaddr *)&serverSock, sizeof(serverSock)) == -1)
	{
		cerr << "Algorithm failure: Cannot connect to Framework Server!" << endl;
		return false;
	}

	cout << ParsedMessage << endl;

	// Odeslání dat
	if ((size = send(mySocket, ParsedMessage.c_str(), ParsedMessage.size() + 1, 0)) == -1)
	{
		cerr << "Algorithm failure: Problem with sending message!" << endl;
		return false;
	}
	cout << "Algorithm info: Message send." << size << endl;

	close(mySocket);
	this->cont->ReturnConnection(this->database->ReturnConnection());
	return true;
}

std::string Algorithm::CreateMessage(){

	xml_document *resp = new xml_document();

	//Definuj element algorithm_message
	xml_node algorithm_message = resp->append_child("alg_m");
	algorithm_message.append_attribute("protocol_version");
	algorithm_message.append_attribute("userID");
	algorithm_message.append_attribute("algID");
	algorithm_message.append_attribute("adapterID");

	//Nastav element algorithm_message
	algorithm_message.attribute("protocol_version") = "1.0";
	algorithm_message.attribute("userID") = this->userID.c_str();
	algorithm_message.attribute("algID") = this->algID.c_str();
	algorithm_message.attribute("adapterID") = this->adapterID.c_str();

	//Dale nejak predavat DeviceId, Device Offset a DeviceType ... je v atributu values

	xml_node notifications = algorithm_message.append_child("notifs");
	notifications.append_attribute("count");
	int countNotifs = this->toNotify.size();
	notifications.attribute("count") = to_string(countNotifs).c_str();  //poèet notifikací

	//Add all notifications to message
	for (auto oneNotif = this->toNotify.begin(); oneNotif != this->toNotify.end(); ++oneNotif){
		xml_node notifNode = notifications.append_child("notif");
		notifNode.append_attribute("type");
		notifNode.append_attribute("text");
		tnotify * notiftmp = *oneNotif;
		notifNode.attribute("type") = to_string(notiftmp->notifyType).c_str();
		notifNode.attribute("text") = notiftmp->notifyText.c_str();
	}

	xml_node toggleActors = algorithm_message.append_child("tactors");
	toggleActors.append_attribute("count");
	int countToggles = this->toToggleActor.size();
	toggleActors.attribute("count") = to_string(countToggles).c_str();  //poèet notifikací

	//Add all toogles to message
	for (auto oneToggle = this->toToggleActor.begin(); oneToggle != this->toToggleActor.end(); ++oneToggle){
		xml_node toggleNode = toggleActors.append_child("notif");
		toggleNode.append_attribute("id");
		toggleNode.append_attribute("type");
		ttoggle * toggleTmp = *oneToggle;
		toggleNode.attribute("id") = (toggleTmp->id).c_str();
		toggleNode.attribute("type") = (toggleTmp->type).c_str();
	}

	tstringXMLwriter writer;
	resp->print(writer);
	delete(resp);
	std::string tmp = writer.result;
	if (tmp[tmp.length() - 1] == '\n')
	{
		tmp.erase(tmp.length() - 1, 1);
	}
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" + tmp;

}
/*
---------------------------------------------------------------------
Do algoritmu se musí pøedat pøes parametry pøík. øádku násl. data:
---------------------------------------------------------------------
const long long int userID;					//ID uživatele algoritmu
const long long int algID;					//ID algoritmu
const long long int adapterID;				//ID adaptéru
const unsigned short int offset;			//Offset algoritmu uživatele na senzoru
const tAlgValues *values;					//Pøedané hodnoty do algoritmu
const vector <std::String> parameters;

typedef struct AlgValues
{
unsigned short int values_count;
tvalue* values;								//Se musí vytvoøit pole hodnot tvalue
unsigned int* sensor_ids;					//Se musí vytvoøit na stejných indexech pole hodnot s ID senzory
} tAlgValues;

argc == 7 

argv[1] ... userID
argv[2] ... algID
argv[3] ... adapterID
argv[4] ... offset
argv[5] ... values
argv[6] ... parameters


*/
Algorithm * Algorithm::getCmdLineArgsAndCreateAlgorithm(int argc, char *argv[]){

	string userIDString = "";
	string algIDString = "";
	string adapterIDString = "";
	string UserAlgIdString = "";
	string valuesString = "";
	string parametersString = "";
	int opt;
	bool u, a, d, o, v, p;
	u = a = d = o = v = p = false;
	while ((opt = getopt(argc, argv, "hu:a:d:o:v:p:")) != EOF)
		switch (opt)
	{
		case 'h':
			cout << "print help" << endl;
			return nullptr;
			break;
		case 'u': // userID
			userIDString = optarg;
			u = true;
			break;
		case 'a': // algID
			algIDString = optarg;
			a = true;
			break;
		case 'd': // adapterID
			adapterIDString = optarg;
			d = true;
			break;
		case 'o': // offset
			UserAlgIdString = optarg;
			o = true;
			break;
		case 'v': // senzor values
			valuesString = optarg;
			v = true;
			break;
		case 'p': // parameters given by User
			parametersString = optarg;
			p = true;
			break;
		default:
			cerr << "Algorithm failure: Wrong command line arguments!\n";
			return nullptr;
	}
	if (!u || !a || !d || !o){
		cerr << "Algorithm failure: Wrong command line arguments! You have to specify at least -u -a -d -o\n";
		return nullptr;
	}

	cerr << "Paramstring: " << parametersString.c_str() << endl;

	//Deklarace promìnných pro uložení z operací parsování
	multimap<unsigned int, map<string, string>> values;
	vector<string> params;
	vector<tRidValues *> Rids;

	//TODO Parsování vstupních øetìzcù ve valuesString a parametersString, pokud je to potøeba
	if (v){
		values = Algorithm::parseValues(valuesString, &Rids);
	}
	if (p){
		params = Algorithm::parseParams(parametersString);
	}
	return new Algorithm(userIDString, algIDString, adapterIDString, UserAlgIdString, values, params, Rids);
}

//Metoda, která vezme hodnotu parametru -v pøík. øádky a zparsuje jej
multimap<unsigned int, map<string, string>> Algorithm::parseValues(std::string values, vector<tRidValues *> *Rids){
	vector<string> senzorValues = Algorithm::explode(values, '$');

	multimap<unsigned int, map<string, string>> valuesTmp;
	
	for (auto it = senzorValues.begin(); it != senzorValues.end(); ++it){
		
		vector<string> tmp = Algorithm::explode(*it, '#');
		map<string, string> tmpmap;
		
		for (auto it2 = tmp.begin(); it2 != tmp.end(); ++it2){
			vector<string> tmp2 = Algorithm::explode(*it2, '=');
			tmpmap[tmp2.front()] = tmp2.back();
		}
		
		string RidOrDevice = tmpmap.at("RidOrDevice");
		if (RidOrDevice.compare("device") == 0){
			string newMultimapIndexStr = tmpmap.at("ID");
			unsigned int newMultimapIndex = std::stoi(newMultimapIndexStr);
			valuesTmp.insert(std::pair<unsigned int, map<string, string>>(newMultimapIndex, tmpmap));
		}
		else if (RidOrDevice.compare("rid") == 0){
			string newRid = tmpmap.at("RID");
			string newtypeRid = tmpmap.at("type");
			tRidValues * newRidValue = new tRidValues();
			newRidValue->rid = newRid;
			newRidValue->type = newtypeRid;
			Rids->push_back(newRidValue);
		}


	}
	return valuesTmp;
}

//Metoda, která vezme hodnotu parametru -p pøík. øádky a zparsuje jej
vector<string> Algorithm::parseParams(std::string paramsInput){
	
	vector<string> params = Algorithm::explode(paramsInput, '#');
	return params;
}

vector<string> Algorithm::explode(string str, char ch) {
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
		else if (*it == '\\' )
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

std::multimap<unsigned int, std::map<std::string, std::string>> Algorithm::getValues() {
	return this->values;
}

std::vector<std::string> Algorithm::getParameters(){
	return this->parameters;
}

std::vector<tRidValues *> Algorithm::getRids(){
	return this->Rids;
}

//Nastavi podminku dle zadaneho retezce
int Algorithm::SetCondition(std::string cond){
	tcondition retVal;

	if (cond.compare("eq") == 0){
		retVal = EQ;
	}
	else if (cond.compare("gt") == 0){
		retVal = GT;
	}
	else if (cond.compare("ge") == 0){
		retVal = GE;
	}
	else if (cond.compare("lt") == 0){
		retVal = LT;
	}
	else if (cond.compare("le") == 0){
		retVal = LE;
	}
	else if (cond.compare("btw") == 0){
		retVal = BTW;
	}
	else if (cond.compare("chg") == 0){
		retVal = CHG;
	}
	else if (cond.compare("dp") == 0){
		retVal = DP;
	}
	else if (cond.compare("time") == 0){
		retVal = TIME;
	}
	else if (cond.compare("geo") == 0){
		retVal = GEO;
	}
	else{

	}

	return retVal;
}
