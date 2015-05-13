/**
* @file uiServerHandle.cpp
*
* @Implementace Metod pro zpracovani zprav od UI serveru
*
* @author xrasov01
* @version 1.0
*/

#include "framework.h"

using namespace std;
using namespace soci;
using namespace pugi;

/** Metoda obsluhujici prichozi spojeni od UI serveru (zprostredkovane od mobilniho zarizeni)
*
*/
void FrameworkServerHandle::HandleUIServerMessage(std::string data, Loger *Log, FrameworkConfig *FConfig, DBFWHandler *database, sem_t* dbAccessSem){
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());

	tstringXMLwriter writer;
	doc.print(writer);
	std::string acceptedMessageString = writer.result;

	if (acceptedMessageString[acceptedMessageString.length() - 1] == '\n')
	{
		acceptedMessageString.erase(acceptedMessageString.length() - 1, 1);
	}

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
		int algIdInt = algMessage.attribute("atype").as_int();
		string algName = algMessage.attribute("algname").value();
		Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : UIServerMessageone addalg");
		Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : atype - " + to_string(algIdInt));
		talgorithm * AlgSpecification = FConfig->GetAlgorithmById(algIdInt);
		Log->WriteMessage(TRACE, "HandleClientConnection: after FConfig->GetAlgorithmById(algIdInt)");

		if (AlgSpecification != nullptr){
			Log->WriteMessage(TRACE, "HandleClientConnection: after if (AlgSpecification != NULL){");

			int maxdevsToExpect = AlgSpecification->maxDevs;
			int maxparamsToExpect = AlgSpecification->numParams;
			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : maxdevsToExpect - " + to_string(maxdevsToExpect));
			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : maxparamsToExpect - " + to_string(maxparamsToExpect));
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
					if (numberOfDevs == maxdevsToExpect){
						Log->WriteMessage(ERR, "HandleClientConnection: UIServerMessage : Wrong number of devs in protocol");
						error = true;
						break;
					}
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
					if (numberOfParams == maxparamsToExpect){
						Log->WriteMessage(ERR, "HandleClientConnection: UIServerMessage : Wrong number of params in protocol");
						error = true;
						break;
					}
					int parPos = algNode.attribute("pos").as_int();
					string parText = algNode.child_value();
					params[numberOfParams].pos = parPos;
					params[numberOfParams].text = parText;
					numberOfParams++;
				}
			}

			if (!(numberOfDevs > maxdevsToExpect || numberOfParams > maxparamsToExpect) && !error){
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
				if (!error){
					stringToSendAsAnswer = this->createMessageAlgCreated(newUserALgorithmId);
				}
				else{
					stringToSendAsAnswer = this->createMessageFalse("12");
				}
			}
			else{
				Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage : incorrect message according to specification of algorithm!");
				error = true;
			}
		}
		else{
			Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage : incorrect message according to specification of algorithm!");
			error = true;
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
		int algIdInt = algMessage.attribute("atype").as_int();

		talgorithm * AlgSpecification = FConfig->GetAlgorithmById(algIdInt);

		if (AlgSpecification != NULL){
			int maxdevsToExpect = AlgSpecification->maxDevs;
			int maxparamsToExpect = AlgSpecification->numParams;
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
					if (numberOfDevs == maxdevsToExpect){
						Log->WriteMessage(ERR, "HandleClientConnection: UIServerMessage : Wrong number of devs in protocol");
						error = true;
						break;
					}
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
					if (numberOfParams == maxparamsToExpect){
						Log->WriteMessage(ERR, "HandleClientConnection: UIServerMessage : Wrong number of params in protocol");
						error = true;
						break;
					}
					int parPos = algNode.attribute("pos").as_int();
					string parText = algNode.child_value();
					params[numberOfParams].pos = parPos;
					params[numberOfParams].text = parText;
					numberOfParams++;
				}
			}

			if (!(numberOfDevs > maxdevsToExpect || numberOfParams > maxparamsToExpect) && !error){
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
			}
			else{
				Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage : incorrect message according to specification of algorithm!");
				error = true;
			}
		}
		else{
			Log->WriteMessage(FATAL, "HandleClientConnection: UIServerMessage : incorrect message according to specification of algorithm!");
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
	else if (state.compare("passborder") == 0){
		//zpracování zprávy passborder
		Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : passborder ENTERING");
		string userAlgId = algMessage.attribute("algid").value();
		string rid = algMessage.attribute("rid").value();
		string typeRid = algMessage.attribute("type").value();
		//Vytahnout algoritmy uzivatele, ktere jsou geofencing a spustit je s parametry
		Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : userId" + userId);
		std::vector<std::string> IDsUserAlg = database->SelectIdsAlgorithmsByAlgIdAndUserId(to_string(3), userId);

		if (!IDsUserAlg.empty()){
			Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : IDsUserAlg not empty");
			for (auto UserAlgId = IDsUserAlg.begin(); UserAlgId != IDsUserAlg.end(); ++UserAlgId){

				string UserId = database->SelectUserIdByUsersAlgId(*UserAlgId);
				string AlgId = database->SelectAlgIdByUsersAlgId(*UserAlgId);
				string parametersTmp = database->SelectParametersByUsersAlgId(*UserAlgId);
				string adapterIdString = database->SelectAdapterIdByUsersAlgId(*UserAlgId);

				Log->WriteMessage(TRACE, "HandleClientConnection: UIServerMessage : IDsUserAlg not empty" + UserId);

				//Prepare senzor values for argument -v
				string senzorValues = "";
				senzorValues += "RidOrDevice=rid#";
				senzorValues += "RID=" + rid + "#";
				senzorValues += "type=" + typeRid + "#";

				string nameOfBinary = "geofencing";

				Log->WriteMessage(INFO, "EXECUTING ALGORITHM BINARY " + nameOfBinary + " - AlgId: " + AlgId + " , userId: " + UserId + ", parameters: " + parametersTmp + ", senzorValues: " + senzorValues);

				char* arg_list[] = {
					StringToChar(nameOfBinary),
					StringToChar("-u"), // userID
					StringToChar(UserId),
					StringToChar("-a"), // algID
					StringToChar(AlgId),
					StringToChar("-d"), // adapterID
					StringToChar(adapterIdString),
					StringToChar("-o"), // UsersAlgId
					StringToChar(*UserAlgId),
					StringToChar("-v"), //Senzor Values
					StringToChar(senzorValues),
					StringToChar("-p"), // parameters given by User
					StringToChar(parametersTmp),
					StringToChar("/"),
					NULL
				};
				this->spawn(StringToChar("Algorithms/geofencing"), arg_list);
			}
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