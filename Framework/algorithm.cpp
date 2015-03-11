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

#define FW_PORT "7282"

// Konstruktor Algoritmu, bude pøedán do algoritmu pod názvem instance alg
Algorithm::Algorithm(long long int init_userID, long long int init_algID, long long int init_adapterID,
	unsigned short int init_offset, multimap<unsigned int, map<string, string>> init_values, std::vector<std::string> init_parameters){

	this->userID = init_userID;
	this->algID = init_algID;
	this->adapterID = init_adapterID;
	this->offset = init_offset;
	this->values = init_values; 
	this->parameters = init_parameters;
}

Algorithm::~Algorithm(){
	
}

// Metoda pøidávající notifikaci uživateli algoritmu
bool Algorithm::AddNotify(unsigned short int type, std::string text){
	/*
	try
	{
		tnotify newNotification = new tnotify();
	}
	catch (std::exception &e)
	{
		newNotification = NULL;
		cerr < "Algorithm failure: Unable to allocate memory for structure tnotify!";
		return false;
	}
	newNotification->notifyType = type;
	newNotification->NotifyText = text;
	this->toNotify.push_back(newNotification);
	*/
	return true;
}

bool Algorithm::AddDataToDB(tvalue *AddValue, unsigned short int offsetInDB){

	return true;
}


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

	// Odeslání dat
	if ((size = send(mySocket, ParsedMessage.c_str(), ParsedMessage.size() + 1, 0)) == -1)
	{
		cerr << "Algorithm failure: Problem with sending message!" << endl;
		return false;
	}
	cout << "Algorithm info: Message send." << size << endl;

	close(mySocket);
	
	return true;
}

std::string Algorithm::CreateMessage(){

	xml_document *resp = new xml_document();

	//Definuj element algorithm_message
	xml_node algorithm_message = resp->append_child("algorithm_message");
	algorithm_message.append_attribute("protocol_version");
	algorithm_message.append_attribute("userID");
	algorithm_message.append_attribute("algID");
	algorithm_message.append_attribute("adapterID");
	//algorithm_message.append_attribute("sensor_id");
	algorithm_message.append_attribute("offset");
	//Nastav element algorithm_message
	algorithm_message.attribute("protocol_version") = "1.0";
	algorithm_message.attribute("userID") = to_string(this->userID).c_str();
	algorithm_message.attribute("algID") = to_string(this->algID).c_str();
	algorithm_message.attribute("adapterID") = to_string(this->adapterID).c_str();
	//algorithm_message.attribute("sensor_id") = to_string(this->sensor_id).c_str();
	algorithm_message.attribute("offset") = to_string(this->offset).c_str();

	xml_node notifications = algorithm_message.append_child("notifications");
	//notifications.attribute("count") = (this->toNotify.size()).c_str();  //poèet notifikací
	notifications.attribute("count") = "0";
	//Pøidej všechny notifikace
	/*
	for (auto oneNotif = this->toNotify.begin(); oneNotif != this->toNotify.end(); ++oneNotif){
		xml_node notifNode = notifications.append_child("notif");
		notifNode.append_attribute("type");
		notifNode.append_attribute("text");
		notifNode.attribute("type") = (oneNotif->notifyType).c_str();
		notifNode.attribute("text") = (oneNotif->notifyText).c_str();
	}
	*/
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
	string offsetString = "";
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
		case 'u':
			userIDString = optarg;
			u = true;
			break;
		case 'a':
			algIDString = optarg;
			a = true;
			break;
		case 'd':
			adapterIDString = optarg;
			d = true;
			break;
		case 'o':
			offsetString = optarg;
			o = true;
			break;
		case 'v':
			valuesString = optarg;
			v = true;
			break;
		case 'p':
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
	long long int userID = std::stoi(userIDString);
	long long int algID = std::stoi(algIDString);
	long long int adapterID = std::stoi(adapterIDString);
	unsigned short int offset = std::stoi(offsetString);
	
	//Deklarace promìnných pro uložení z operací parsování
	multimap<unsigned int, map<string, string>> values;
	vector<string> params;

	//TODO Parsování vstupních øetìzcù ve valuesString a parametersString, pokud je to potøeba
	if (v){
		values = Algorithm::parseValues(valuesString);
	}
	if (p){
		params = Algorithm::parseParams(parametersString);
	}
	return new Algorithm(userID, algID, adapterID, offset, values, params);
}

//Metoda, která vezme hodnotu parametru -v pøík. øádky a zparsuje jej
multimap<unsigned int, map<string, string>> Algorithm::parseValues(std::string values){
	vector<string> senzorValues = Algorithm::explode(values, '$');

	multimap<unsigned int, map<string, string>> valuesTmp;
	
	for (auto it = senzorValues.begin(); it != senzorValues.end(); ++it){
		
		vector<string> tmp = Algorithm::explode(*it, '#');
		map<string, string> tmpmap;
		
		for (auto it2 = tmp.begin(); it2 != tmp.end(); ++it2){
			vector<string> tmp2 = Algorithm::explode(*it2, '=');
			tmpmap[tmp2.front()] = tmp2.back();
		}
		
		string newMultimapIndexStr = tmpmap.at("ID");
		unsigned int newMultimapIndex = std::stoi(newMultimapIndexStr);
		valuesTmp.insert(std::pair<unsigned int, map<string, string>>(newMultimapIndex, tmpmap));
	}
	return valuesTmp;
}

//Metoda, která vezme hodnotu parametru -p pøík. øádky a zparsuje jej
vector<string> Algorithm::parseParams(std::string paramsInput){
	vector<string> params = Algorithm::explode(paramsInput, '#');

	/* COMMENTED - NEED	TO WRITE METHOD SpaceReplace
	vector<string> paramsOutput;
	
	for (auto it = params.begin(); it != params.end(); ++it){
		string tmp = Algorithm::spaceReplace(*it);
		paramsOutput.push_back(tmp);
	}
	return paramsOutput;
	*/
	return params;
}
//TODO:
//Replace \_ in string with comma
std::string Algorithm::spaceReplace(std::string text) {


	return text;
}

vector<string> Algorithm::explode(string str, char ch) {
	string next;
	vector<string> result;

	// For each character in the string
	for (string::const_iterator it = str.begin(); it != str.end(); it++) {
		// If we've hit the terminal character
		if (*it == ch) {
			// If we have some characters accumulated
			if (!next.empty()) {
				// Add them to the result vector
				result.push_back(next);
				next.clear();
			}
		}
		else {
			// Accumulate the next character into the sequence
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