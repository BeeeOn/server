/**
* @file algorithm.h
*
* Hlavickovy soubor pro kontejner pro praci s databazi (pomoci knihovny soci).
*
*/

#ifndef DATABASECONNECTIONCONTAINER_H_
#define DATABASECONNECTIONCONTAINER_H_

#include "loger.h"
#include <mutex>
#include <soci.h>
#include <postgresql/soci-postgresql.h>

/** Trida, ktera je kontejnerem pro kolekci databazovych spojeni (sessions). Z teto kolekce si pak aplikace modularniho prostredi muze vzit dle potreby jednu z jiz predem vytvorenych databazovych spojeni bez nutneho navazovani spojeni na databazi a tak zbrzdovani provedeni databazoveho dotazu.
*
*/
class DBConnectionsContainer
{
	private:
		static DBConnectionsContainer *instance;			/** Ukazatel na objekt DBConnectionsContainer. Je zde vyuzit navrhovy vzor jedinacek. */
		Loger *_log;										/** Ukazatel na objekt pomoci nehoz se provadi logovani zaznamu do logovaciho souboru. */
		const std::string _Name = "DBConnectionsContainer";	/** Nazev tridy pro lepsi pouziti v implementaci tridy. */
		int freeCount;										/** Pocet volnych databazovych sezeni (uvnitr kontejneru). */
		soci::session *connections[100];					
		std::mutex semaphore;								
		std::string _NameOfDB;								/** Nazev databaze. */
		DBConnectionsContainer(std::string DBName, int ConnLimit, Loger *l); 
	public:
		static DBConnectionsContainer *GetConnectionContainer(Loger *l, std::string DBName, int ConnLimit);
		~DBConnectionsContainer();
		soci::session *GetConnection();
		void ReturnConnection(soci::session *conn);
		int Limit();
};
#endif /* DATABASECONNECTIONCONTAINER_H_ */