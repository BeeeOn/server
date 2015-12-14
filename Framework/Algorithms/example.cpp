/**
* @file watch_and_notify.cpp
*
* @Implementace prvního algoritmu watch_and_notify
* @Oèekávané parametry:
*						SENZOR_VALUE1 | ANY_SENZOR_VALUE(ID SENZORU)
*						OPERATOR | STRING
*						VALUE | REAL
*						NOTIFICATION | STRING
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
	/*Tìlo programu*/



	/*Konec tìla programu*/
	//Odeslání dat do Frameworku a ukonèení algoritmu.
	if (!alg->SendAndExit()){
		delete(alg);
		return EXIT_FAILURE;
	}
	delete(alg);
	return EXIT_SUCCESS;
}