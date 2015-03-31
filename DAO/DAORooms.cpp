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

DAORooms::DAORooms(){
}


DAORooms::~DAORooms() {
}

DAORooms& DAORooms::getInstance(){
        static DAORooms instance;
        return instance;
}


int DAORooms::insertNewRoom(string adapterId, string roomType, string roomName)
{
    Logger::getInstance(Logger::DEBUG3)<< "insert new room\n";
    
    try
    {
        soci::session sql(*_pool);
        //TODO zamknout row, zmenit id na DB typ serial?
       int newRoomId;
       /*  sql << "select max(room_id) from rooms where fk_adapter_id = "+adapterId, soci::into(newRoomId);
        newRoomId++;*/
        sql << "insert into rooms (fk_adapter_id, type, name) values( :a_id, :type, :name) returning room_id",
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

void DAORooms::updateRoom(string adapterId, string roomId, string type, string name){
    Logger::getInstance(Logger::DEBUG3)<<"DB: "<< "update room\n";
    
    try
    {
        soci::session sql(*_pool);
        
        statement st = (sql.prepare <<  "update rooms set type=:type , name=:name where room_id=:id and fk_adapter_id= :a_id;",
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
void DAORooms::deleteRoom(string adapterId, string roomId){
    Logger::getInstance(Logger::DEBUG3)<<"DB: "<< "delete room\n";
    
    try
    {
        soci::session sql(*_pool);
        
        sql << "delete from rooms where room_id=:id and fk_adapter_id=:a_id ",
                use(roomId, "id"), use(adapterId, "a_id");
        
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }  
}
string DAORooms::getXMLrooms(string adapterId)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"roomsList (adapter="<<adapterId<<")\n";
    try{
            soci::session sql(*_pool);

            string xml;
            indicator ind;
            sql<<"select xmlagg("
                    "xmlelement(name loc , "
                        "xmlattributes(room_id as id, type as type, name as name) "
                    ")" 
            ")"
            "from rooms where fk_adapter_id =:adapter and room_id is not null" 
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