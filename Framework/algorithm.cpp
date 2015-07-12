/**
* @file algorithm.cpp
*
* Implementace metod pro tvorbu aplikacniho modulu.
*
* @author Martin Rasovsky (xrasov01@stud.fit.vutbr.cz)
* @version 1.0
*/
#include "algorithm.h"

using namespace std;
using namespace pugi;
using namespace soci;

#define FW_PORT "7084"

/** Konstruktor tridy Algorithm.
*
*/
Algorithm::Algorithm(std::string init_userID, std::string init_algID, std::string init_adapterID,
	std::string init_offset, multimap<unsigned int, map<string, string>> init_values,
	std::vector<std::string> init_parameters, vector<tRidValues *> init_Rids, std::string init_nameOfDB, std::string init_frameworkServerPort){

	this->userID = init_userID;
	this->algID = init_algID;
	this->adapterID = init_adapterID;
	this->offset = init_offset;
	this->values = init_values;
	this->parameters = init_parameters;
	this->Rids = init_Rids;
	this->nameOfDB = init_nameOfDB;
	try{
        this->frameworkServerPort = init_frameworkServerPort;
	}
    catch(std::exception &e){
		std::cerr << e.what() << std::endl;
		std::cerr << "Algorithm::Algorithm Unable to convert port given to short int (16 bits)" << std::endl;
		exit(EXIT_FAILURE);
    }
	try
	{
		this->Log = new Loger();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "Algorithm::Algorithm Unable to create memory space for logging exiting!" << std::endl;
		exit(EXIT_FAILURE);
	}
	//Nastaveni kontejneru pro DB
	this->cont = DBConnectionsContainer::GetConnectionContainer(Log, this->nameOfDB, 1);
	this->Log->SetLogger(7, 5, 100, "algorithm_log",".", "ALGORITHM");
	session *Conn = NULL;
	Conn = cont->GetConnection();
	if (Conn == NULL)
	{
		cout << "Algorithm::Algorithm Unable to get Database connection!" << endl;
	}

	try
	{
		this->database = new DBFWHandler(Conn, Log);
	}
	catch (std::exception &e)
	{
		cout << "Algorithm::Algorithm Unable to create memory space for DBHandler!!!" << endl;
	}
}

/** Destruktor tridy Algorithm.
*
*/
Algorithm::~Algorithm(){
	this->cont->ReturnConnection(this->database->GetConnectionSession());
	delete(this->cont);
	delete(Log);
	//Clean memory of vector toNotify
	for (auto oneNotif = this->toNotify.begin(); oneNotif != this->toNotify.end(); ++oneNotif){
		delete(*oneNotif);
	}
	//Clean memory of vector toToggleActor
	for (auto oneToggle = this->toToggleActor.begin(); oneToggle != this->toToggleActor.end(); ++oneToggle){
		delete(*oneToggle);
	}
	//Clean memory of vector Rids
	for (auto oneRid = this->Rids.begin(); oneRid != this->Rids.end(); ++oneRid){
		delete(*oneRid);
	}
}

/** Metoda umoznujici zaslani notifikace uzivateli, pro ktereho byl aplikacni modul spusten. Jakmile je metoda zavolana, nelze vratit operaci zpet (je potreba na to predem myslet).
*
* @param type			Typ notifikacni zpravy (pro budouci pouziti).
* @param text			Text k zaslani uzivateli.
* @param senzorId		Specifikace pro ktery senzor je notifikacni zprava navazana.
* @param typeOfSenzor	Specifikace pro jaky typ senzoru je notifikacni zprava navazana.
*
* @return boolean o vysledku oprerace
*/
bool Algorithm::AddNotify(unsigned short int type, std::string text, std::string senzorId, std::string typeOfSenzor){
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
	newNotification->senzorId = senzorId;
	newNotification->typeOfSenzor = typeOfSenzor;
	this->toNotify.push_back(newNotification);

	return true;
}

/** Metoda menici stav aktoru dle jeho id a typu. Tyto informace jsou predany uzivateli v ramci parametru (argument -p) a values (argument -v).
*
* @param id			Jedinecny identifikator zarizeni.
* @param type		Typ zarizeni dle tabulky uvedene na strankach orijektu inteligentni domacnosti.
*
* @return Boolean o vysledku oprerace.
*/
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

/** Funkce provede spojeni s modularnim prostredim, odesle zpravu. (Nasledne by se mela radne uvolnit pamet pred ukoncenim celeho aplikacniho modulu.)
*
* @return Boolean o vysledku operace.
*/
bool Algorithm::SendAndExit(){

	string ParsedMessage = this->CreateMessage();
	hostent *host;
	sockaddr_in serverSock;
	int mySocket;
	int port;
	int size;
	try{
        port = atoi(this->frameworkServerPort.c_str());
	}
    catch(std::exception &e){
		std::cerr << e.what() << std::endl;
		std::cerr << "Algorithm::SendAndExit Unable to convert port given to short int (16 bits), port using:"<< this->frameworkServerPort << std::endl;
		return false;
    }

	host = gethostbyname("localhost");

	if ((mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
	{
		cerr << "Algorithm::SendAndExit Cannot to create socket! (port using: "<< port << ")"<< endl;
		return false;
	}
	serverSock.sin_family = AF_INET;
	serverSock.sin_port = htons(port);
	memcpy(&(serverSock.sin_addr), host->h_addr, host->h_length);

	if (connect(mySocket, (sockaddr *)&serverSock, sizeof(serverSock)) == -1)
	{
		cerr << "Algorithm::SendAndExit Algorithm failure! Cannot connect to Framework Server! (port using: "<< port << ")"<< endl;
		return false;
	}

	// Odeslání dat
	if ((size = send(mySocket, ParsedMessage.c_str(), ParsedMessage.size() + 1, 0)) == -1)
	{
		cerr << "Algorithm::SendAndExit Algorithm failure! Problem with sending message!" << endl;
		return false;
	}
	close(mySocket);

	return true;
}

/** Vytvori zpravu k odeslani na modularni prostredi pomoci externi knihovny soci.
*
* @return Vytvorena zprava k odeslani na modularni prostredi.
*/
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
		notifNode.append_attribute("senzorId");
		notifNode.append_attribute("typeOfSenzor");
		tnotify * notiftmp = *oneNotif;
		notifNode.attribute("type") = to_string(notiftmp->notifyType).c_str();
		notifNode.attribute("text") = notiftmp->notifyText.c_str();
		notifNode.attribute("senzorId") = notiftmp->senzorId.c_str();
		notifNode.attribute("typeOfSenzor") = notiftmp->typeOfSenzor.c_str();
	}

	xml_node toggleActors = algorithm_message.append_child("tactors");
	toggleActors.append_attribute("count");
	int countToggles = this->toToggleActor.size();
	toggleActors.attribute("count") = to_string(countToggles).c_str();  //poèet notifikací

	//Add all toogles to message
	for (auto oneToggle = this->toToggleActor.begin(); oneToggle != this->toToggleActor.end(); ++oneToggle){
		xml_node toggleNode = toggleActors.append_child("tactor");
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
/** Staticka metoda zpracujici parametry prikazoveho radku a vytvarejici objekt tridy Algorithm.
*
* @return Algorithm * Ukazatel na vytvoreny objekt tridy Algorithm.
*/
Algorithm * Algorithm::getCmdLineArgsAndCreateAlgorithm(int argc, char *argv[]){

	string userIDString = "";
	string algIDString = "";
	string adapterIDString = "";
	string UserAlgIdString = "";
	string valuesString = "";
	string parametersString = "";
	string nameOfDB = "";
	string portOfFrameworkServer = "";
	int opt;
	bool u, a, d, o, v, p, e, s;
	u = a = d = o = v = p = e = s = false;
	while ((opt = getopt(argc, argv, "hu:a:d:o:v:p:e:s:")) != EOF)
		switch (opt)
	{
		case 'h':
			Algorithm::usage(argv[0]);
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
		case 'e': // name of database to use
			nameOfDB = optarg;
			e = true;
			break;
		case 's': // socket port to send answer to framework server
			portOfFrameworkServer = optarg;
			s = true;
			break;
		default:
			cerr << "Algorithm failure: Wrong command line arguments!\n";
			return nullptr;
	}
	if (!u || !a || !d || !o){
		cerr << "Algorithm failure: Wrong command line arguments! You have to specify at least -u -a -d -o\n";
		return nullptr;
	}

	if(!e){
        cerr << "Algorithm failure: Wrong command line arguments! No specified -e cmd arg for DB Name!\n";
		return nullptr;
	}
	if(!s){
        cerr << "Algorithm failure: Wrong command line arguments! No specified -s cmd arg for port of Framework server!\n";
		return nullptr;
	}

	//Deklarace promìnných pro uložení z operací parsování
	multimap<unsigned int, map<string, string>> values;
	vector<string> params;
	vector<tRidValues *> Rids;

	if (v){
		values = Algorithm::parseValues(valuesString, &Rids);
	}
	if (p){
		params = Algorithm::parseParams(parametersString);
	}
	return new Algorithm(userIDString, algIDString, adapterIDString, UserAlgIdString, values, params, Rids, nameOfDB, portOfFrameworkServer);
}

void Algorithm::usage(char* progName)
{
  cout << progName << "[options]" << endl <<
      "Options:" << endl <<
      "-h      Print this help" << endl <<
      "-u      userID" << endl <<
      "-a      algorithmID" << endl <<
      "-d      adapterId" << endl <<
      "-d      adapterId" << endl <<
      "-v      senzor Values in format RidOrDevice=device#ID=3976200203#type=10#fval=23.900000#offset=0" << endl <<
      "-p      user parameters in format 3976200203---10#lt#24#notif#mensi 24" << endl <<
      "-e      name of Database" << endl;
}

/** Metoda, ktera vezme hodnotu parametru -v prikazove radky a zparsuje jej.
*
*/
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
			unsigned int newMultimapIndex = std::atoll(newMultimapIndexStr.c_str());
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

/** Metoda, ktera vezme hodnotu parametru -p prikazove radky a zparsuje jej.
*
*/
vector<string> Algorithm::parseParams(std::string paramsInput){

	vector<string> params = Algorithm::explode(paramsInput, '#');
	return params;
}

vector<string> Algorithm::explode(string str, char ch) {
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
		else if (*it == '\\' )
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

/** Metoda, ktera vrati atribut values.
*
*/
std::multimap<unsigned int, std::map<std::string, std::string>> Algorithm::getValues() {
	return this->values;
}

/** Metoda, ktera vrati atribut parameters.
*
*/
std::vector<std::string> Algorithm::getParameters(){
	return this->parameters;
}

/** Metoda, ktera vrati atribut Rids.
*
*/
std::vector<tRidValues *> Algorithm::getRids(){
	return this->Rids;
}

/** Metoda, ktera nastavi podminku dle zadaneho retezce.
* @param cond podminka v retezci
*
* @return podminka dle enumu.
*/
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
