/**
* @file algorithm.h
*
* @Definice t��d pro Algoritmy
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

//Struktura p�ed�vaj�c� informace ohledn� notifikace
typedef struct notify
{
	unsigned short int notifyType;	//Typ notifikace (Bude pou�ito do budoucna, zat�m m��eme vlo�it 0 jako obecn� notifikace)
	std::string NotifyText;			//Text zpr�vy
} tnotify;

typedef struct AlgValues
{
	unsigned short int values_count;
	tvalue* values;
	unsigned int* sensor_ids;
} tAlgValues;

typedef std::multimap<unsigned int, std::map<std::string, std::string>> values_t;

/** @brief Class Algorithm

T��da kter� je poskytnuta pro tvorbu a obsluhu algoritmu.
*/
class Algorithm {
private:
	long long int userID;					//ID u�ivatele algoritmu
	long long int algID;					//ID algoritmu
	long long int adapterID;				//ID adapt�ru
	unsigned short int offset;			//Offset algoritmu u�ivatele na senzoru
	values_t values;					//P�edan� hodnoty do algoritmu
	std::vector<std::string> parameters;		//Parametry algoritmu, po�ad� atd. si definuje autor algoritmu
	std::vector<tnotify *> toNotify;				//Vektor uchov�vaj�c� jednotliv� notifikace (kdyby z n�jak�ho d�vodu jich bylo v�ce)		
	std::vector<tvalue *> addValues;				//Hodnoty, kter� se p�idaj� do DB
	
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
	//TODO Zp��stupnit DB + Aktory
	/*
	int SetActor();
	int ToggleActor();
	tvalue * GetLastAlgorithmData();
	*/
};