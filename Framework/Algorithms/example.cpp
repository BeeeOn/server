/**
* @file watch_and_notify.cpp
*
* @Implementace prvn�ho algoritmu watch_and_notify
* @O�ek�van� parametry:
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
	//Deklarace a inicializace objektu Algorithm poskytuj�c� rozhran� pro tvorbu algoritmu.
	Algorithm *alg;
	if ((alg = Algorithm::getCmdLineArgsAndCreateAlgorithm(argc, argv)) == nullptr){
		return EXIT_FAILURE;
	}
	/*T�lo programu*/



	/*Konec t�la programu*/
	//Odesl�n� dat do Frameworku a ukon�en� algoritmu.
	if (!alg->SendAndExit()){
		delete(alg);
		return EXIT_FAILURE;
	}
	delete(alg);
	return EXIT_SUCCESS;
}