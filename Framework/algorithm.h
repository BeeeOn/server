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

#include "structures.h"
#include "../pugixml.hpp"


typedef enum condition
{
	EQ,			//equal - hodnoty jsou stejn�
	GT,			//greater than - hodnota(senzoru) je v�t�� ne� 
	GE,			//greater equal - hodnota(senzoru) je vet�� nebo stejn�
	LT,			//lesser than - hodnota(senzoru) je men�� ne�
	LE,			//lesser equal - hodnota(senzoru) je men�� nebo stejn�
	BTW,		//between - hodnota senzoru je v intervalu (mezi hodnotami)
	CHG,		//change - hodnota se zm�nila
	DP,			//dew point - rosn� bod
	TIME,		//time - �ek� na dan� �as
	GEO,		//geofence position - �ek� a� bude v dan� lokalit�
} tcondition;

typedef struct AlgValues
{
	unsigned short int values_count;
	tvalue* values;
	unsigned int* sensor_ids;
} tAlgValues;

typedef std::multimap<unsigned int, std::map<std::string, std::string>> values_t;

//Struktura p�ed�vaj�c� informace ohledn� notifikace
typedef struct notify
{
	unsigned short int notifyType;	//Typ notifikace (Bude pou�ito do budoucna, zat�m m��eme vlo�it 0 jako obecn� notifikace)
	std::string notifyText;			//Text zpr�vy
} tnotify;

typedef struct toggle
{
	std::string id;
	std::string type;
} ttoggle;

/** @brief Class Algorithm

T��da kter� je poskytnuta pro tvorbu a obsluhu algoritmu.
*/
class Algorithm {
private:
	std::string userID;							//ID u�ivatele algoritmu
	std::string algID;							//ID algoritmu
	std::string adapterID;						//ID adapt�ru
	std::string offset;							//UsersAlgorithmsId
	std::multimap<unsigned int, std::map<std::string, std::string>> values;							//P�edan� hodnoty do algoritmu
	std::vector<std::string> parameters;		//Parametry algoritmu, po�ad� atd. si definuje autor algoritmu
	std::vector<tnotify *> toNotify;			//Vektor uchov�vaj�c� jednotliv� notifikace (kdyby z n�jak�ho d�vodu jich bylo v�ce)		
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
	//TODO Zp��stupnit DB + Aktory
	/*
	int SetActor();
	int ToggleActor();
	tvalue * GetLastAlgorithmData();
	*/
};