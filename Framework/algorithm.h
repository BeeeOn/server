/**
* @file algorithm.h
*
* @Definice tøíd pro Algoritmy
*
* @author xrasov01
* @version 1.0
*/

#include <vector>
#include <iostream>
#include <string>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <iterator>
#include <exception>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <map>
#include <regex>

#include "structures.h"
#include "../pugixml.hpp"


typedef enum condition
{
	EQ,			//equal - hodnoty jsou stejné
	GT,			//greater than - hodnota(senzoru) je vìtší než 
	GE,			//greater equal - hodnota(senzoru) je vetší nebo stejná
	LT,			//lesser than - hodnota(senzoru) je menší než
	LE,			//lesser equal - hodnota(senzoru) je menší nebo stejná
	BTW,		//between - hodnota senzoru je v intervalu (mezi hodnotami)
	CHG,		//change - hodnota se zmìnila
	DP,			//dew point - rosný bod
	TIME,		//time - èeká na daný èas
	GEO,		//geofence position - èeká až bude v dané lokalitì
} tcondition;

typedef struct AlgValues
{
	unsigned short int values_count;
	tvalue* values;
	unsigned int* sensor_ids;
} tAlgValues;

typedef std::multimap<unsigned int, std::map<std::string, std::string>> values_t;

//Struktura pøedávající informace ohlednì notifikace
typedef struct notify
{
	unsigned short int notifyType;	//Typ notifikace (Bude použito do budoucna, zatím mùžeme vložit 0 jako obecná notifikace)
	std::string notifyText;			//Text zprávy
} tnotify;

typedef struct toggle
{
	std::string id;
	std::string type;
} ttoggle;

/** @brief Class Algorithm

Tøída která je poskytnuta pro tvorbu a obsluhu algoritmu.
*/
class Algorithm {
private:
	std::string userID;							//ID uživatele algoritmu
	std::string algID;							//ID algoritmu
	std::string adapterID;						//ID adaptéru
	std::string offset;							//UsersAlgorithmsId
	std::multimap<unsigned int, std::map<std::string, std::string>> values;							//Pøedané hodnoty do algoritmu
	std::vector<std::string> parameters;		//Parametry algoritmu, poøadí atd. si definuje autor algoritmu
	std::vector<tnotify *> toNotify;			//Vektor uchovávající jednotlivé notifikace (kdyby z nìjakého dùvodu jich bylo více)		
	std::vector<ttoggle *> toToggleActor;
	
	static std::multimap<unsigned int, std::map<std::string, std::string>> parseValues(std::string values);
	static std::vector<std::string> parseParams(std::string paramsInput);
	static std::string spaceReplace(std::string text);
public:
	Algorithm(std::string init_userID, std::string init_algID, std::string init_adapterID,
		std::string init_offset, std::multimap<unsigned int, std::map<std::string, std::string>> init_values, std::vector<std::string> init_parameters);
	~Algorithm();
	bool AddNotify(unsigned short int type, std::string text);
	//bool AddDataToDB(tvalue *AddValue, unsigned short int offsetInDB);
	bool SendAndExit();
	std::string CreateMessage();
	std::multimap<unsigned int, std::map<std::string, std::string>> getValues();
	std::vector<std::string> getParameters();
	static Algorithm * getCmdLineArgsAndCreateAlgorithm(int argc, char *argv[]);
	int SetCondition(std::string cond);
	static std::vector<std::string> explode(std::string str, char ch);
	bool ChangeActor(std::string id, std::string type);
	//TODO Zpøístupnit DB + Aktory
	/*
	int SetActor();
	int ToggleActor();
	tvalue * GetLastAlgorithmData();
	*/
};