/**
* @file adapterServerHandle.cpp
*
* @Implementace Metod pro zpracovani zprav od Adapter Serveru
*
* @author xrasov01
* @version 1.0
*/

#include "framework.h"

using namespace std;
using namespace soci;
using namespace pugi;

/** Metoda obsluhujici prichozi spojeni od Adapter Reciever Serveru.
*
*/
void FrameworkServerHandle::HandleAdapterMessage(std::string data, Loger *Log, FrameworkConfig *FConfig, DBFWHandler *database, sem_t* dbAccessSem){
	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());

	tstringXMLwriter writer;
	doc.print(writer);
	std::string acceptedMessageString = writer.result;

	if (acceptedMessageString[acceptedMessageString.length() - 1] == '\n')
	{
		acceptedMessageString.erase(acceptedMessageString.length() - 1, 1);
	}

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

	if (!allIdsOfUsersAlgorithms.empty()){
		Log->WriteMessage(TRACE, "HandleClientConnection: AdapterServerMessage - after condition(!allIdsOfUsersAlgorithms.empty()) && parsedMessage->state != 0 ");
			//Nalezen alespon jeden UserAlg s danym AdapterId
			for (auto idOfUsersAlgorithms = allIdsOfUsersAlgorithms.begin(); idOfUsersAlgorithms != allIdsOfUsersAlgorithms.end(); ++idOfUsersAlgorithms){
				vector<string> allSenzorIds = database->SelectDevIdsByUsersAlgId(*idOfUsersAlgorithms);
				if (!allSenzorIds.empty()){
					Log->WriteMessage(TRACE, "HandleClientConnection: AdapterServerMessage - after SECOND condition (! allSenzorIds.empty())");
					//Nasel mac pro UserAlg
					for (auto itSenzorId = allSenzorIds.begin(); itSenzorId != allSenzorIds.end(); ++itSenzorId){
						int currentDevId = strtol((*itSenzorId).c_str(), NULL, 10);
						Log->WriteMessage(TRACE, "HandleClientConnection: AdapterServerMessage - THIRD THESE TWO - currentDevId:" + to_string(currentDevId) + ", deviceIdInt: " + to_string(deviceIdInt));

						if (currentDevId == deviceIdInt){
							for (int i = 0; i < this->parsedMessage->values_count; i++)
							{
								//  The argument list to pass to the Algorithm
								string UserId = database->SelectUserIdByUsersAlgId(*idOfUsersAlgorithms);
								string AlgId = database->SelectAlgIdByUsersAlgId(*idOfUsersAlgorithms);
								string parametersTmp = database->SelectParametersByUsersAlgId(*idOfUsersAlgorithms);
								string parameters = '"' + parametersTmp + '"';
								int AlgIdInt = std::stoi(AlgId);
								talgorithm * algorithmSpecification = FConfig->GetAlgorithmById(AlgIdInt);

								if (algorithmSpecification != NULL){
									//Prepare senzor values for argument -v
									string senzorValues = "";
									senzorValues += "RidOrDevice=device#";
									senzorValues += "ID=" + (*itSenzorId) + "#";
									senzorValues += "type=" + to_string(this->parsedMessage->values[i].type) + "#";
									if (this->parsedMessage->values[i].type == TEMP || this->parsedMessage->values[i].type == HUM){ //Predavani do fval
										senzorValues += "fval=" + to_string(this->parsedMessage->values[i].fval) + "#";
									}
									else { //Predavani do fval
										senzorValues += "ival=" + to_string(this->parsedMessage->values[i].fval) + "#";
									}

									senzorValues += "offset=" + to_string(this->parsedMessage->values[i].offset);

									Log->WriteMessage(INFO, "EXECUTING ALGORITHM BINARY " + algorithmSpecification->name + " - AlgId: " + AlgId + " , userId: " + UserId + ", parameters: " + parametersTmp + ", senzorValues: " + senzorValues);

									char* arg_list[] = {
										StringToChar(algorithmSpecification->name),
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

									cout << algorithmSpecification->pathOfBinary.c_str() << endl;
									this->spawn(StringToChar(algorithmSpecification->pathOfBinary), arg_list);

									Log->WriteMessage(INFO, "EXECUTED ALGORITHM BINARY " + algorithmSpecification->name + "- AlgId: " + AlgId + "userId: " + UserId + ", parameters: " + parametersTmp + ", senzorValues: " + senzorValues);
								}
							}
						}
					}
				}
			}
		}
}