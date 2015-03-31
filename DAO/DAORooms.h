/* 
 * File:   DAORooms.h
 * Author: pavel
 *
 * Created on 29. březen 2015, 21:26
 */

#ifndef DAOROOMS_H
#define	DAOROOMS_H

#include "DAO.h"

class DAORooms : public DAO {
private:
    DAORooms();
    DAORooms(DAORooms const&);// Don't Implement
    void operator=(DAORooms const&); // Don't implement
public:
    static DAORooms& getInstance();
    ~DAORooms(void);

    int insertNewRoom(std::string adapterId, std::string roomType, std::string roomName);
    void updateRoom(std::string adapterId, std::string roomId, std::string type, std::string name);
    void deleteRoom(std::string adapterId, std::string roomId);
    std::string getXMLrooms(std::string adapterId);
};

#endif	/* DAOROOMS_H */

