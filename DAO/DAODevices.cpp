#include "DAODevices.h"
#include "ServerException.h"
#include "DAOGateways.h"
#include "DAOMobileDevices.h"
#include <postgresql/soci-postgresql.h>
using namespace std;
using namespace soci;
const string DAODevices::tableDevices = "device";
const string DAODevices::tableModule = "module";

const DeviceColumns DAODevices::col;
const ModuleColumns DAODevices::moduleCol;

DAODevices::DAODevices() 
{
}

DAODevices::~DAODevices() 
{
}

string DAODevices::getXMLDevicesQueryString()
{
    return " xmlagg("
                        "xmlelement(name device,"
                                    "xmlattributes(" + col.euid + " as euid, " + col.id + " as type, " + col.location_id + " as locationid, " + tableDevices+"."+col.status + " as status, "
                                                " " + col.measured_at + " as time, " + col.involved + "  as involved, " + col.name + "  as name, " + col.gateway_id + " as gateid),"
                                    "(select xmlagg(xmlelement(name module,xmlattributes(" + moduleCol.id + " as id, " + moduleCol.value + " as value))) "
                                            "from " + tableModule + " where " + tableModule+"."+moduleCol.euid + "=" + tableDevices+"."+ col.euid + 
                                                    " AND " + tableModule+"."+moduleCol.gateway_id +"="+tableDevices+"."+col.gateway_id + " )"
                    ")"
                ") ";
}

std::string DAODevices::getXMLAllDevs(gatewayId_64b gateId)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getCommunicationXml"<<"\n";
        
        try
        {
            soci::session sql(*_pool);
            
            std::string xml;
            indicator ind;
            
            sql << "select " + getXMLDevicesQueryString() +
                    "from  " +  tableDevices + 
                    " where " + col.gateway_id + "=:gate and " + DAODevices::col.init + "!='0' " 
                    ,use(gateId,"gate"),soci::into(xml,ind);
            //cout << sql.get_last_query();

        if(ind != i_ok){
                Logger::getInstance(Logger::DEBUG3)<<"query not ok"<<"\n";
               return "";         
            }
            return xml;
        }
        catch (soci::postgresql_soci_error& e)
        {
            Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
            throw;
        }
}

string DAODevices::getXMLdevices(int userId, vector<gatewayId_64b> gateVector, vector<deviceId_64b> devicesVec)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getPartialDevices"<<"\n";
    try
    {
        soci::session sql(*_pool);

        string xml;
        indicator ind;
                
        if(gateVector.size() == 0 || devicesVec.size() == 0){
            Logger::debug3()<<"empty vectors" << endl;
            return "";
        }
        Logger::debug3()<<"gates:"<<vectorToPsqlNotation(gateVector)<<"\n";
        Logger::debug3()<<"devs:"<<vectorToPsqlNotation(devicesVec)<<"\n";
        
        string gates = vectorToPsqlNotation(gateVector);
        string devices = vectorToPsqlNotation(devicesVec);
        
        
        
        statement st = (sql.prepare <<
                
                "select  " <<
                getXMLDevicesQueryString() +
               "from user_gateway left join gateway using(gateway_id) left join " + tableDevices + " using(gateway_id)" <<
                "where " +col.gateway_id+" in(" + gates + ") and " + col.euid + " in(" + devices + ") and user_id=:userId",
                use(userId),
                soci::into(xml, ind) );
        
        
        
        cout<<endl<<sql.get_last_query()<<endl;
        st.execute(true);
        return xml;
    }
    catch (postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        throw;
    }
}

string DAODevices::getXMLNewDevices(gatewayId_64b gateId)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getPartialDevices"<<"\n";
        try
        {
                soci::session sql(*_pool);
                
                
                string xml;
                indicator ind;
                statement st = (sql.prepare <<"select " +
                        getXMLDevicesQueryString() +
                       "from " + tableDevices + " where "+col.init+"='0' and " +col.gateway_id+" =:gate" 
                        ,use(gateId, "gate"),soci::into(xml, ind) );
                
                st.execute(true);
                             
                return xml;
                
        }
        catch (postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}

int DAODevices::updateFacility(gatewayId_64b gateId, string id, string init, string locationId, string name) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update Facility"<<id<<"\n";
        soci::session sql(*_pool); 
    try
    {      
        
        string columnsToSet;
        indicator locationInd = i_ok;
        if(locationId == "null")
            locationInd = i_null;
        if(init != "")columnsToSet.append(" "+col.init+"=:init, ");
        if(locationId != "")columnsToSet.append(" "+col.location_id+"=:locId, ");
        if(name != "")columnsToSet.append(" "+col.name+"=:name, ");
        columnsToSet = columnsToSet.substr(0,columnsToSet.size()-2);//erase last ', '
        
        statement st = (sql.prepare << "update " << tableDevices << " set " <<
                columnsToSet <<
                " where "+col.euid+" = :mac AND "+col.gateway_id+"=:gateId",
                use(init, "init"),use(locationId, locationInd, "locId"),use(name, "name"),use(id, "mac"), use(gateId, "gateId")
                );
        
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        cout << sql.get_last_query();
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}