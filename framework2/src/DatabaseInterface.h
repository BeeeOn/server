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
     * Constructor of class DatabaseInterface.
     * DO NOT USE! Use getInstance() instad! Must be present for std::make_shared template.
     */
    DatabaseInterface();
    
    /** Copy constructor is deleted. */
    DatabaseInterface(const DatabaseInterface& orig) = delete;
    
    /** Destructor of class DatabaseInterface. */
    virtual ~DatabaseInterface();
    
    /** Connection pool object. Maintains threads to connect to database. */
    std::shared_ptr<soci::connection_pool> m_connection_pool;
    
    /**
     * Returns pointer to singleton instance of class DatabaseInterface.
     * @return Pointer to singleton instance.
     */
    static std::shared_ptr<DatabaseInterface> getInstance();

    /**
     * Creates connectin pool and connects to database.
     * @param sessions_count Number of session threads.
     * @param connection_string String containing formated information to connect to database.
     */
    void connectToDatabase(int sessions_count, std::string connection_string);
    
    /**
     * Makes new session to database and returns poiter to it.
     * @return Shared pointer to new session.
     */
    std::shared_ptr<soci::session> makeNewSession();
    
private:

    /**
     * Singleton instance pointer.
     */
    static std::shared_ptr<DatabaseInterface> m_instance;
};

#endif /* DATABASEINTERFACE_H */
