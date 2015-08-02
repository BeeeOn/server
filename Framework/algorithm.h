/**
* @file algorithm.h
*
* Definice trid pro tvorbu aplikacnich modulu.
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
#include "loger.h"
#include "DBConnectionsContainer.h"
#include "DBFWHandler.h"
#include "structures.h"
#include "pugixml.hpp"

/** Enum condition uchovavajici hodnoty reprezentujici operatory ci entity pouzivane v aplikacnich modulech (>, <, =, >=, <=, ...).
*
*/
typedef enum condition
{
	EQ,			/** equal - hodnoty jsou stejné */
	GT,			/** greater than - hodnota(senzoru) je vìtší než */
	GE,			/** greater equal - hodnota(senzoru) je vetší nebo stejná */
	LT,			/** lesser than - hodnota(senzoru) je menší než */
	LE,			/** lesser equal - hodnota(senzoru) je menší nebo stejná */
	BTW,		/** between - hodnota senzoru je v intervalu (mezi hodnotami) */
	CHG,		/** change - hodnota se zmìnila */
	DP,			/** dew point - rosný bod */
	TIME,		/** time - èeká na daný èas */
	GEO,		/** geofence position - èeká až bude v dané lokalitì */
} tcondition;

/** Struktura k uchovavani hodnoty predane ze senzoru (jiz realne, nikoliv jen jejich ID).
*
*/
typedef struct AlgValues
{
	unsigned short int values_count;	/** Pocet predanych hodnot ze senzoru do Aplikacniho modulu */
	tvalue* values;						/** Ukazatel na pole hodnot predanych ze senzoru a ulozenych ve strukturach tvalue. */
	unsigned int* sensor_ids;			/** Ukazatel na pole identifikacnich cisel senzoru, ze kterych tyto hodnoty prisly. */
} tAlgValues;

/** Struktura k uchovavani informaci ohledne geolokacni oblasti.
*
*/
typedef struct RidValues
{
	std::string rid;	/** Identifikacni retezec geolokacni oblasti. */
	std::string type;	/** Smer prekroceni dane oblasti, ktera se muze hlidat v ramci aplikacniho modulu. */
} tRidValues;

typedef std::multimap<unsigned int, std::map<std::string, std::string>> values_t;

/** Struktura k uchovavani informaci ohledne notifikace. K naslednemu vytvoreni zpravy a odeslani na server modularniho prostredi.
*
*/
typedef struct notify
{
	unsigned short int notifyType;	/** Typ notifikace (Bude pouzito do budoucna, zatim muzeme vlozit 0 jako obecna notifikace)	*/
	std::string notifyText;			/** Text zpravy */
	std::string senzorId;			/** Identifikacni cislo/retezec senzoru, ktery je vazan na notifikaci. Nasledne se pri otevreni notifikace v mobilnim zarizeni muze prejit z teto notifikace na konkretni senzor. */
	std::string typeOfSenzor;		/** Typ senzoru, ktery je vazan na notifikaci viz predchozi atribut senzorId. */
} tnotify;

/** Struktura k uchovavani informaci ohledne zmeny aktoru.
*
*/
typedef struct toggle
{
	std::string id;		/** Identifikacni cislo/retezec aktoru, ktery bude menit svuj stav. */
	std::string type;	/** Typ aktoru, ktery bude menit svuj stav. Dle enumu na webovych strankach inteligentni domacnost. */
} ttoggle;

/** Trida ktera je poskytnuta pro tvorbu a obsluhu algoritmu.
*
*/
class Algorithm {
private:
	std::string userID;							/** ID uzivatele algoritmu */
	std::string algID;							/** ID algoritmu */
	std::string adapterID;						/** ID adapteru	 */
	std::string offset;							/** UsersAlgorithmsId */
	std::string nameOfDB;						/** Nazev databaze k pripojeni. */
	std::string DBUser;							/** ID uzivatele algoritmu */
	std::string DBPassword;							/** ID uzivatele algoritmu */
	std::multimap<unsigned int, std::map<std::string, std::string>> values;							/**Predane hodnoty do algoritmu. */
	std::vector<std::string> parameters;		/** Parametry algoritmu, poøadí atd. si definuje autor algoritmu. */
	std::vector<tnotify *> toNotify;			/** Vektor uchovavajici jednotlive notifikace (kdyby z nejakeho duvodu jich bylo vice). */
	std::vector<ttoggle *> toToggleActor;		/** Vektor uchovavajici jednotlive zmeny aktoru (kdyby z nejakeho duvodu jich bylo vice). */
	std::vector<tRidValues *> Rids;				/** Vektor uchovavajici geolokacni oblasti. */
	std::string frameworkServerPort;
	Loger *Log;									/** Loger pro logování do souboru. */
	DBConnectionsContainer *cont = NULL;		/** Container pro DB. */
	static std::multimap<unsigned int, std::map<std::string, std::string>> parseValues(std::string values, std::vector<tRidValues *> *Rids);
	static std::vector<std::string> parseParams(std::string paramsInput);
	static std::string spaceReplace(std::string text);
public:
	Algorithm(std::string init_userID, std::string init_algID, std::string init_adapterID,
	std::string init_offset, std::multimap<unsigned int, std::map<std::string, std::string>> init_values,
	std::vector<std::string> init_parameters, std::vector<tRidValues *> init_Rids, std::string init_nameOfDB, std::string init_frameworkServerPort,
	std::string init_DBUser, std::string init_DBPassword);
	~Algorithm();
	bool AddNotify(unsigned short int type, std::string text, std::string senzorId, std::string typeOfSenzor);
	bool SendAndExit();
	std::string CreateMessage();
	std::multimap<unsigned int, std::map<std::string, std::string>> getValues();
	std::vector<std::string> getParameters();
	std::vector<tRidValues *> getRids();
	static Algorithm * getCmdLineArgsAndCreateAlgorithm(int argc, char *argv[]);
	static void usage(char* progName);
	int SetCondition(std::string cond);
	static std::vector<std::string> explode(std::string str, char ch);
	bool ChangeActor(std::string id, std::string type);
	DBFWHandler *database;						/** Pristup do databaze. */
};
