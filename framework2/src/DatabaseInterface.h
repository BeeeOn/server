/* 
 * File:   DatabaseInterface.h
 * Author: Martin Novak, xnovak1c@stud.fit.vutbr.cz
 *
 * Created on 23. March 2016
 */

#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include <memory>

#include <soci.h>
#include <postgresql/soci-postgresql.h> 

typedef std::shared_ptr<soci::session> SessionSharedPtr;

class DatabaseInterface{
public:
    /**
     * Creates DatabaseInterface singleton instance.
     * Must be called just once in entire program.
     */
    static void createInstance();
    /**
     * Returns pointer to singleton instance of class DatabaseInterface.
     * @return Pointer to singleton instance.
     */
    static std::shared_ptr<DatabaseInterface> getInstance();
    /**
     * Destructor of class DatabaseInterface.
     */
    virtual ~DatabaseInterface();
    /**
     * Creates connectin pool and connects all work threads to database.
     * @param sessions_count Number of session threads.
     * @param connection_string String containing formated information to connect to database.
     */
    void connectToDatabase(int sessions_count, std::string connection_string);
    /**
     * Makes new session to database on pool and returns poiter to it.
     * @return Shared pointer to new session.
     */
    SessionSharedPtr makeNewSession();

private:
    /**
     * Constructor of class DatabaseInterface.
     */
    DatabaseInterface();
    /**
     * Delete copy constructor.
     */
    DatabaseInterface(const DatabaseInterface& orig) = delete;
    /**
     * Delete assignment operator.
     */
    void operator=(const DatabaseInterface&) = delete;
    /**
     * Singleton instance pointer.
     */
    static std::shared_ptr<DatabaseInterface> m_instance;
    /**
     * Connection pool object. Maintains threads to connect to database.
     */
    std::shared_ptr<soci::connection_pool> m_connection_pool;
};

#endif /* DATABASEINTERFACE_H */
