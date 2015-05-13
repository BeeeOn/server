/**
* @file watch_and_notify.cpp
*
* @Implementace prvního algoritmu watch_and_notify
* @Ocekavane parametry:
*	RID | STRING
*   TYPE | DIRECTION
*   NOTIF_OR_ACTOR | NOTIF_OR_ACTOR
*   NOTIFICATION | STRING
* @author xrasov01
* @version 1.0
*/
#include <cstdlib>
#include "../algorithm.h"

using namespace std;

int main(int argc, char *argv[])
{
	//Deklarace a inicializace objektu Algorithm poskytující rozhraní pro tvorbu algoritmu.
	Algorithm *alg;
	if ((alg = Algorithm::getCmdLineArgsAndCreateAlgorithm(argc, argv)) == nullptr){
		return EXIT_FAILURE;
	}
	/*-----------------------------------Tìlo programu-----------------------------------*/

	//Zpracovani parametru
	string idOfRIDParams;
	int idOfRIDParamsInt;
	string directionParams;
	string notificationTextOrIdActor = "";
	string toBeNotifiedOrChangeActor = "";
	bool notifVersionOfAlgorithm = false;
	bool actorVersionOfAlgorithm = false;
	vector<string> parameters = alg->getParameters();

	int parameterCounter = 1;
	for (auto parameter = parameters.begin(); parameter != parameters.end(); ++parameter){
		if (parameterCounter == 1){ //RID | STRING
			idOfRIDParams = (*parameter);
			//idOfRIDParamsInt = std::atoll(idOfRIDParams.c_str());

		}
		else if (parameterCounter == 2){ // TYPE | DIRECTION
			directionParams = (*parameter);
		}
		else if (parameterCounter == 3){
			//do nothing
		}
		else if (parameterCounter == 4){  // NOTIF_OR_ACTOR | NOTIF_OR_ACTOR
			toBeNotifiedOrChangeActor = (*parameter);
			if (toBeNotifiedOrChangeActor.compare("notif") == 0){
				notifVersionOfAlgorithm = true;
			}
			else{
				actorVersionOfAlgorithm = true;
			}
		}
		else if (parameterCounter == 5){ // NOTIFICATION OR ACTOR | STRING
			notificationTextOrIdActor = *parameter;
		}
		else{
			delete(alg);
			return EXIT_FAILURE;
		}
		parameterCounter++;
	}

	string idOfActor = "";
	string typeOfActor = "";

	if (actorVersionOfAlgorithm){

		std::string delimeter = "---";
		idOfActor = notificationTextOrIdActor.substr(0, notificationTextOrIdActor.find(delimeter)); 

		std::string tmpString = notificationTextOrIdActor.substr(notificationTextOrIdActor.find(delimeter)); 
		typeOfActor = tmpString.erase(0, delimeter.length());
	}

	//Zpracovani hodnot Rid

	std::vector<tRidValues *> Rids = alg->getRids();
	string directionValues;
	string idOfRIDValues;
	for (auto it = Rids.begin(); it != Rids.end(); ++it)
	{
		idOfRIDValues = (*it)->rid;
		directionValues = (*it)->type;
	}

	bool isToBeSendNotificationOrChangeActor = false;

	//Test whether the algorithm should do something
	if ((directionParams.compare(directionValues) == 0) && (idOfRIDParams.compare(idOfRIDValues) == 0)){
		isToBeSendNotificationOrChangeActor = true;
	}

	if (isToBeSendNotificationOrChangeActor){
		if (notifVersionOfAlgorithm){
			//Odeslat notifikaci
			alg->AddNotify(1, notificationTextOrIdActor, "", "");
		}
		if (actorVersionOfAlgorithm){
			//Zmenit aktor
			alg->ChangeActor(idOfActor, typeOfActor);
		}
	}
	/*-----------------------------------Konec tìla programu-----------------------------------*/
	//Odeslání dat do Frameworku a ukonèení algoritmu.
	if (!alg->SendAndExit()){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}