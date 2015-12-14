#include "DAOLocations.h"

#include "../ui_logger/Logger.h"
#include "ServerException.h"
using namespace std;
using namespace soci;

const LocationsColumns  DAOLocations::col;
const string DAOLocations::tableLocations = "location";

DAOLocations::DAOLocations()
{
}

DAOLocations::~DAOLocations() 
{
}

int DAOLocations::insertNewRoom(gatewayId_64b gateId, int roomType, string roomName)
{
    Logger::getInstance(Logger::DEBUG3)<< "insert new room\n";
    
    try
    {
        soci::session sql(*_pool);
        int newRoomId;
        sql << "insert into " << tableLocations << " (" << col.gateway_id << ", " << col.kind << ", " << col.name << ") values( :a_id, :type, :name) returning " << col.id << " ",
                use(gateId, "a_id"), use(roomType, "type"), use(roomName, "name"), into(newRoomId);
    
        Logger::getInstance(Logger::DEBUG3)<< "inserted\n";
        return newRoomId;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }   
}

int DAOLocations::update(gatewayId_64b gatewayId, int roomId, int type, string name)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB: "<< "update room\n";
    
    try
    {
        soci::session sql(*_pool);
        
        statement st = (sql.prepare <<  "update " << tableLocations << " set " << col.kind << "=:type , " << col.name << "=:name where " << col.id << "=:id and " << col.gateway_id << "= :a_id;",
                use(type, "type"),use(name, "name"), use(roomId, "id"), use(gatewayId, "a_id") );
        st.execute(true);
        
        int updateCount = st.get_affected_rows();
        Logger::getInstance(Logger::DEBUG3)<<"update"<<updateCount<<endl;
        return updateCount;
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }  
}

void DAOLocations::remove(gatewayId_64b gatewayId, string roomId)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB: "<< "delete room\n";
    
    try
    {
        soci::session sql(*_pool);
        
        sql << "delete from " << tableLocations << " where " << col.id << "=:id and " << col.gateway_id << "=:a_id ",
                use(roomId, "id"), use(gatewayId, "a_id");
        
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }  
}

string DAOLocations::getXMLrooms(gatewayId_64b gatewayId)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"roomsList (gate="<<gatewayId<<")\n";
    try{
            soci::session sql(*_pool);

            string xml;
            indicator ind;
            sql<<"select xmlagg("
                    "xmlelement(name location , "
                        "xmlattributes(" << col.id << " as id, " << col.kind << " as type, " << col.name << " as name) "
                    ")" 
            ")"
            "from " << tableLocations << " where " << col.gateway_id << " =:gateway and " << col.id << " is not null" 
            ,use(gatewayId,"gateway"),soci::into(xml, ind);
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