/**
* @file config.h
*
* Definice tridy FrameworkConfig
*
* @author xrasov01
* @version 1.0
*/

#include <string>
#include <stdlib.h>
#include "loger.h"
#include "DBFWHandler.h"

#ifndef STRUCTURES_CONFIG_
#define STRUCTURES_CONFIG_

/** Struktura uchovavajici informace o definici jednoho aplikacniho modulu.
*
*/
typedef struct algorithm
{
	int id;									/** Identifikacni cislo aplikacniho modulu. */
	std::string name;						/** Nazev aplikacniho modulu (binarni soubor). */
	std::string pathOfBinary;				/** Cesta k binarnimu souboru. */
	int numParams;							/** Maximalni pocet parametru pro aplikacni modul (pokud by bylo vice, vyskytla by se chyba). */
	int maxDevs;							/** Maximalni pocet zarizeni asociovanych s aplikacnim modulem (pokud by jich bylo vice, vyskytla by se chyba). */
	int type;								/** Typ aplikacniho modulu (1 - aplikacni modul spousteny pri prichodu hodnot ze senzoru, 2 - aplikacni modul spousteny pri prichodu zprav z mobilnich zarizeni). */
} talgorithm;

/** Trida pro nacteni a uchovavani konfigurace modularniho prostredi dle definovaneho XML souboru.
*
*/
class FrameworkConfig
{
private:
	Loger *Log;								/** Ukazatel na objekt pro logovani do souboru. */
public:
	int portUIServer;						/** Port pro vytvoreni serveru pro zpracovani zprav od UI Serveru (respektive od mobilnich zarizeni). */
	int portAdaRecieverServer;				/** Port pro vytvoreni serveru pro zpracovani zprav od Ada Reciever Serveru (zpracovani prichodu novych dat ze senzoru). */
	int portAdaSenderServer;				/** Port, na kterem nasloucha Ada Server Sender a na ktery se odesilaji pozadavky o zmene aktoru. */
	int portAlgorithmServer;				/** Port pro vytvoreni serveru pro zpracovani zprav od aplikacnich modulu. */
	int recieveBuffSize;					/** Maximalni velikost bufferu pri prijeti dat. */
	int maxNumberDBConnections;				/** Pocet sezeni nad databazi ulozenych do kontejneru databazovych spojeni. */
	std::string dbName;						/** Nazev lokalni databaze, na kterou se modularni prostredi pripojuje. */
	std::string dbUser;
	std::string dbPassword;
	std::string algorithmsConfig;			/** Cesta ke konfiguracnimu souboru aplikacnich modulu se seznamem jejich definic. */
	std::string loggerSettingFileName;		/** Prefix pro logovaci soubor. */
	std::string loggerSettingAppName;		/** Nazev aplikace pro nastaveni pro logovani - v tomto pripade FRAMEWORK nebo MODULAR INTERFACE. */
	int loggerSettingVerbosity;				/** Nastaveni iry, dle ktere se maji vypisovat zaznamy do logovaciho souboru. */
	int loggerSettingFilesCnt;				/** Maximalni pocet vytvorenych logovacich souboru. */
	int loggerSettingLinesCnt;				/** Maximalni pocet radku v logovacim souboru. */
	std::vector<talgorithm *> algorithms;	/** Kolekce struktur talgorithm uchovavajici informace o definicich aplikacnich modulu. */
	int connectionPoolSize;

	FrameworkConfig();
	~FrameworkConfig();
	void SetConfig(std::string configPath);
	void SetAlgorithms();
	void ClearAlgorithms();
	void SetLogger(Loger *init_Log);
	void ResetAlgorithms();
	talgorithm * GetAlgorithmById(int id);
	void SetUpAlgorithmsInDatabase(DBFWHandler * database);

};

#endif
