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

#include "../structures.h"
#include "../pugixml.hpp"

//Struktura pøedávající informace ohlednì notifikace
typedef struct notify
{
	unsigned short int notifyType;	//Typ notifikace (Bude použito do budoucna, zatím mùžeme vložit 0 jako obecná notifikace)
	std::string NotifyText;			//Text zprávy
} tnotify;

typedef struct AlgValues
{
	unsigned short int values_count;
	tvalue* values;
	unsigned int* sensor_ids;
} tAlgValues;

typedef std::multimap<unsigned int, std::map<std::string, std::string>> values_t;

/** @brief Class Algorithm

Tøída která je poskytnuta pro tvorbu a obsluhu algoritmu.
*/
class Algorithm {
private:
	long long int userID;					//ID uživatele algoritmu
	long long int algID;					//ID algoritmu
	long long int adapterID;				//ID adaptéru
	unsigned short int offset;			//Offset algoritmu uživatele na senzoru
	values_t values;					//Pøedané hodnoty do algoritmu
	std::vector<std::string> parameters;		//Parametry algoritmu, poøadí atd. si definuje autor algoritmu
	std::vector<tnotify *> toNotify;				//Vektor uchovávající jednotlivé notifikace (kdyby z nìjakého dùvodu jich bylo více)		
	std::vector<tvalue *> addValues;				//Hodnoty, které se pøidají do DB
	
	static std::multimap<unsigned int, std::map<std::string, std::string>> parseValues(std::string values);
	static std::vector<std::string> parseParams(std::string paramsInput);
	static std::vector<std::string> explode(std::string str, char ch);
	static std::string spaceReplace(std::string text);
public:
	Algorithm(long long int init_userID, long long int init_algID, long long int init_adapterID,
		unsigned short int init_offset, std::multimap<unsigned int, std::map<std::string, std::string>> init_values, std::vector<std::string> init_parameters);
	~Algorithm();
	bool AddNotify(unsigned short int type, std::string text);
	bool AddDataToDB(tvalue *AddValue, unsigned short int offsetInDB);
	bool SendAndExit();
	std::string CreateMessage();
	std::multimap<unsigned int, std::map<std::string, std::string>> getValues();
	std::vector<std::string> getParameters();
	static Algorithm * getCmdLineArgsAndCreateAlgorithm(int argc, char *argv[]);
	//TODO Zpøístupnit DB + Aktory
	/*
	int SetActor();
	int ToggleActor();
	tvalue * GetLastAlgorithmData();
	*/
};