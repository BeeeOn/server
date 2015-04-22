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

namespace sc = std::chrono;

void FrameworkServerHandle::HandleAlgorithmMessage(std::string data, Loger *Log, FrameworkConfig *FConfig){
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
	Log->WriteMessage(TRACE, "MESSAGE ACCEPTED: " + acceptedMessageString);
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
				Notification *notif = new LimitExceededNotification(userID, 0, *IDs, now, notifyText, adapterIdInt, "111", 1, 2);
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
	Log->WriteMessage(TRACE, "EXITING FrameworkServerHandle::HandleAlgorithmMessage");
}