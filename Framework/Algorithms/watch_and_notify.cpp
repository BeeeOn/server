/**
* @file watch_and_notify.cpp
*
* @Implementace prvního algoritmu watch_and_notify
* @Ocekavane parametry:
*	SENZOR_VALUE1 | ANY_SENZOR_VALUE
*	OPERATOR | ENTITY
*	VALUE | REAL
*	NOTIF_OR_ACTOR | NOTIF_OR_ACTOR
*	NOTIFICATION | STRING
*
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
	string idOfSenzorString = "";
	string typeOfSenzorString = "";
	unsigned int idOfSenzor;
	int typeOfSenzor;
	int op;
	double valueDefinedByUser;
	string notificationTextOrIdActor = "";
	string toBeNotifiedOrChangeActor = "";
	bool notifVersionOfAlgorithm = false;
	bool actorVersionOfAlgorithm = false;
	vector<string> parameters = alg->getParameters();

	int parameterCounter = 1;
	for (auto parameter = parameters.begin(); parameter != parameters.end(); ++parameter){
		if (parameterCounter == 1){
			std::string AnySenzorValue = (*parameter);
			std::string delimiter = "---";
			int countTmp = 1;
			size_t pos = 0;
			std::string token;
			while ((pos = AnySenzorValue.find(delimiter)) != std::string::npos) {
				token = AnySenzorValue.substr(0, pos);
				if (countTmp == 1){
					idOfSenzorString = token;
				}
				else if (countTmp == 2){
					typeOfSenzorString = token;
				}
				else{
					break;
				}
				AnySenzorValue.erase(0, pos + delimiter.length());
				countTmp++;
			}
			idOfSenzor = std::atoll(idOfSenzorString.c_str());
		}
		else if (parameterCounter == 2){
			op = alg->SetCondition(*parameter);
		}
		else if (parameterCounter == 3){
			valueDefinedByUser = atof((*parameter).c_str());
		}
		else if (parameterCounter == 4){
			toBeNotifiedOrChangeActor = (*parameter);
			if (toBeNotifiedOrChangeActor.compare("notif") == 0){
				notifVersionOfAlgorithm = true;
			}
			else{
				actorVersionOfAlgorithm = true;
			}
		}
		else if (parameterCounter == 5){
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
		idOfActor = notificationTextOrIdActor.substr(0, notificationTextOrIdActor.find(delimeter)); // token is "scott"

		std::string tmpString = notificationTextOrIdActor.substr(notificationTextOrIdActor.find(delimeter)); // token is "---tiger"
		typeOfActor = tmpString.erase(0, delimeter.length());
	}
	
	//Zpracovani hodnot senzoru
	multimap<unsigned int, map<string, string>> values = alg->getValues();
	map<string, string> oneSenzor;
	
	for (auto it = values.begin(); it != values.end(); ++it)
	{
		unsigned int idOfSenzorFromValues = (*it).first;
		if (idOfSenzor == idOfSenzorFromValues){
			oneSenzor = (*it).second;
			break;
		}
	}

	double fvalFromSenzor;
	int ivalFromSenzor;
	bool IsSetFval = false;
	bool IsSetIval = false;

	for (auto it = oneSenzor.begin(); it != oneSenzor.end(); ++it)
	{
		string key = (*it).first;
		string secValue = (*it).second;
		if (key.compare("fval") == 0){
			fvalFromSenzor = atof(secValue.c_str());
			IsSetFval = true;
			break;
		}
		if (key.compare("ival") == 0){
			ivalFromSenzor = std::stoi(secValue);
			IsSetIval = true;
			break;
		}
	}

	bool isToBeSendNotificationOrChangeActor = false;

	float lastValue = alg->database->GetLastTemp(idOfSenzorString, typeOfSenzorString);

	if (IsSetFval){
		if (op == GT){
			if (fvalFromSenzor > valueDefinedByUser && lastValue <= valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
		else if (op == EQ){
			if (fvalFromSenzor == valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
		else if (op == GE){
			if (fvalFromSenzor >= valueDefinedByUser && lastValue < valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
		else if (op == LT){
			if (fvalFromSenzor < valueDefinedByUser && lastValue >= valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
		else if (op == LE){
			if (fvalFromSenzor <= valueDefinedByUser && lastValue > valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
	}

	if (IsSetIval){
		if (op == GT){
			if (ivalFromSenzor > valueDefinedByUser && lastValue <= valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
		else if (op == EQ){
			if (ivalFromSenzor == valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
		else if (op == GE){
			if (ivalFromSenzor >= valueDefinedByUser && lastValue < valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
		else if (op == LT){
			if (ivalFromSenzor < valueDefinedByUser && lastValue >= valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
		else if (op == LE){
			if (ivalFromSenzor <= valueDefinedByUser && lastValue > valueDefinedByUser){
				isToBeSendNotificationOrChangeActor = true;
			}
		}
	}

	if (isToBeSendNotificationOrChangeActor){
		if (notifVersionOfAlgorithm){
			//Odeslat notifikaci
			alg->AddNotify(1, notificationTextOrIdActor);
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