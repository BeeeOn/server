/**
* @file adapterServerHandle.cpp
*
* Implementace Metod pro zpracovani zprav od Adapter Serveru
*
* @author xrasov01
* @version 1.0
*/

#include "framework.h"

using namespace std;
using namespace soci;
using namespace pugi;

namespace sc = std::chrono;

/** Metoda obsluhujici prichozi spojeni od Algoritmu (Aplikacnich modulu)
*
*/
void FrameworkServerHandle::HandleAlgorithmMessage(std::string data, Loger *Log, FrameworkConfig *FConfig, DBFWHandler *database, sem_t *dbAccessSem){
	Log->WriteMessage(TRACE, "ENTERING FrameworkServerHandle::HandleAlgorithmMessage");

	xml_document doc;
	xml_parse_result result = doc.load_buffer(data.data(), data.size());

	tstringXMLwriter writer;
	doc.print(writer);
	std::string acceptedMessageString = writer.result;

	if (acceptedMessageString[acceptedMessageString.length() - 1] == '\n')
	{
		acceptedMessageString.erase(acceptedMessageString.length() - 1, 1);
	}

	//Kód zpracující zprávy od Algoritmù
	xml_node algMessage = doc.child("alg_m");

	string userID = algMessage.attribute("userID").value();
	int algID = algMessage.attribute("algID").as_int();
	string adapterID = algMessage.attribute("adapterID").value();
	
	
	int algTypeId = algMessage.attribute("algTypeId").as_int();
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

		std::vector<std::string> IDs = database->GetNotifStringByUserId(userID);
		if (!IDs.empty()){
			for (int i = 0; i <= notifsIndex; i++){
				string notifyText = notification.attribute("text").value();
				string senzorId = notification.attribute("senzorId").value();
				int typeOfSenzor = notification.attribute("typeOfSenzor").as_int();
				//ziskat unikatni ID notifikace pomoci DB
				sem_wait(dbAccessSem);
				string currentHighestIdOfNotifPerUserString = database->GetHighestIdInNotification();
				int highestNotifId = atoi(currentHighestIdOfNotifPerUserString.c_str()) + 1;

				Notification *notif = new WatchdogNotif(
					std::stoi(userID),      // User ID ziskane
					highestNotifId,			// Notification ID (melo by byt ve vetsine pripadu unikatni, jinak se prepise jiz existujici notifikace s existujicim ID)
					now,					// Unix timestamp (ms), kdy byla zprava vytvorena; !!!!UTC format!!!
					notifyText,				// Zprava, ktera bude zobrazena uzivateli
					adapterIdInt,			// Adapter ID
					senzorId,				// Senzor ID
					typeOfSenzor,			// Typ + offset
					algID				    // ID Typu algoritmu (tabulka algorithms)
					);

				//Odeslani notifikace a test zda se odeslalo na vsechny GCM Identifikacni retezce (resp. na vsechny zarizeni) a pripadne vymazani neodeslanych GCMIDs z databaze
				vector<string> notSended = notif->sendGcm(&IDs);
				if ( !notSended.empty() ){
					for (auto it = notSended.begin(); it != notSended.end(); ++it){
						database->DeleteMobileDeviceByGcmId(*it);
					}
				}

				if (notif->saveToDb()) {
					// nazvy promenych odpovidaji nazvum sloupcu v DB
					string text = notif->getDbXml();
					string level = to_string(notif->getLevel());
					string userId = to_string(notif->getUserId());
					string timestamp = to_string(notif->getTime());
					string message_id = to_string(notif->getId());
					string name = notif->getName();
					string read = to_string(0);
					// ulozeni do DB
					database->InsertNotification(text, level, timestamp, userId, message_id, read, name);
				}  
				sem_post(dbAccessSem);
				delete notif;
				notification = notification.next_sibling();
			}
		}
		else{
			Log->WriteMessage(FATAL, "HandleClientConnection: AlgorithmMessage: No GCMid for UserId");
		}
	}

	xml_node togleActors = algMessage.child("tactors");
	int togglesCnt = togleActors.attribute("count").as_uint();
	xml_node toggle = togleActors.first_child();

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
	Log->WriteMessage(TRACE, "EXITING FrameworkServerHandle::HandleAlgorithmMessage");
}