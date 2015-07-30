/* 
 * File:   DAORooms.h
 * Author: pavel
 *
 * Created on 29. březen 2015, 21:26
 */

#ifndef DAOROOMS_H
#define	DAOROOMS_H

#include "DAO.h"

struct LocationsColumns{
    std::string id;
    std::string gateway_id;
    std::string kind;
    std::string name;
    
    LocationsColumns():
            id("location_id"), 
            gateway_id("gateway_id"), 
            kind("kind"), 
            name("location_name")
            { }
};

class DAORooms : public DAO {
private:
    DAORooms();
    DAORooms(DAORooms const&);// Don't Implement
    void operator=(DAORooms const&); // Don't implement
public:
    static DAORooms& getInstance();
    ~DAORooms(void);
    
    static const std::string tableLocations;
    static const LocationsColumns col;

    int insertNewRoom(long long adapterId, std::string roomType, std::string roomName);
    void updateRoom(long long adapterId, std::string roomId, std::string type, std::string name);
    void deleteRoom(long long adapterId, std::string roomId);
    std::string getXMLrooms(long long adapterId);
};

#endif	/* DAOROOMS_H */

