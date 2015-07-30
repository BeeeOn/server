/* 
 * File:   DAORooms.cpp
 * Author: pavel
 * 
 * Created on 29. březen 2015, 21:26
 */

#include "DAORooms.h"

#include "../ui_logger/Logger.h"
#include "ServerException.h"
using namespace std;
using namespace soci;

const LocationsColumns  DAORooms::col;
const string DAORooms::tableLocations = "location";

DAORooms::DAORooms(){
}


DAORooms::~DAORooms() {
}

DAORooms& DAORooms::getInstance(){
        static DAORooms instance;
        return instance;
}


int DAORooms::insertNewRoom(long long adapterId, string roomType, string roomName)
{
    Logger::getInstance(Logger::DEBUG3)<< "insert new room\n";
    
    try
    {
        soci::session sql(*_pool);
        //TODO zamknout row, zmenit id na DB typ serial?
       int newRoomId;
       /*  sql << "select max(room_id) from rooms where fk_adapter_id = "+adapterId, soci::into(newRoomId);
        newRoomId++;*/
        sql << "insert into " << tableLocations << " (" << col.gateway_id << ", " << col.kind << ", " << col.name << ") values( :a_id, :type, :name) returning " << col.id << " ",
                use(adapterId, "a_id"), use(roomType, "type"), use(roomName, "name"), into(newRoomId);
    
        Logger::getInstance(Logger::DEBUG3)<< "inserted\n";
        return newRoomId;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }   
}

void DAORooms::updateRoom(long long adapterId, string roomId, string type, string name){
    Logger::getInstance(Logger::DEBUG3)<<"DB: "<< "update room\n";
    
    try
    {
        soci::session sql(*_pool);
        
        statement st = (sql.prepare <<  "update " << tableLocations << " set " << col.kind << "=:type , " << col.name << "=:name where " << col.id << "=:id and " << col.gateway_id << "= :a_id;",
                use(type, "type"),use(name, "name"), use(roomId, "id"), use(adapterId, "a_id") );
        st.execute(true);
        
        int updateCount = st.get_affected_rows();
        Logger::getInstance(Logger::DEBUG3)<<"update"<<updateCount<<endl;
        if(updateCount == 0)
            throw ServerException(ServerException::MISSING_ENTITY);
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }  
}
void DAORooms::deleteRoom(long long adapterId, string roomId){
    Logger::getInstance(Logger::DEBUG3)<<"DB: "<< "delete room\n";
    
    try
    {
        soci::session sql(*_pool);
        
        sql << "delete from " << tableLocations << " where " << col.id << "=:id and " << col.gateway_id << "=:a_id ",
                use(roomId, "id"), use(adapterId, "a_id");
        
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }  
}
string DAORooms::getXMLrooms(long long adapterId)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"roomsList (adapter="<<adapterId<<")\n";
    try{
            soci::session sql(*_pool);

            string xml;
            indicator ind;
            sql<<"select xmlagg("
                    "xmlelement(name loc , "
                        "xmlattributes(" << col.id << " as id, " << col.kind << " as type, " << col.name << " as name) "
                    ")" 
            ")"
            "from " << tableLocations << " where " << col.gateway_id << " =:adapter and " << col.id << " is not null" 
            ,use(adapterId,"adapter"),soci::into(xml, ind);
            //cout<<"xml"<<xml<<endl;
            if(ind != i_ok)
                    return "";
            return xml;
    }
    catch (soci::postgresql_soci_error& e)
    {
            Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
            throw;
    }
}