/* 
 * File:   DAODevices.cpp
 * Author: pavel
 * 
 * Created on 27. březen 2015, 13:33
 */

#include "DAODevices.h"
#include "ServerException.h"
#include "DAOAdapters.h"
#include "DAOMobileDevices.h"
#include <postgresql/soci-postgresql.h>
using namespace std;
using namespace soci;
const string DAODevices::tableDevices = "device";
const string DAODevices::tableModule = "module";

const DeviceColumns DAODevices::col;
const ModuleColumns DAODevices::moduleCol;

DAODevices::DAODevices() {
}


DAODevices::~DAODevices() {
}

DAODevices& DAODevices::getInstance(){
        static DAODevices instance;
        return instance;
}

//string DAODevices::getXMLDevicesQueryStringNewer(string facilitiesCond, string gateways)
//{
//    return "select "
//            " xmlagg("
//                        "xmlelement(name  dev,xmlattributes(" + col.init + " as init, " + col.mac + " as did, " + col.id + " as type, " + col.location_id + " as lid, " + col.refresh + " as refresh, "
//                                                " " + col.measured_at + " as time, " + col.involved + "  as inv, " + col.name + "  as name),"
//                                    "(select xmlagg(xmlelement(name part,xmlattributes(" + moduleCol.id + " as type, " + moduleCol.value + " as val))) "
//                                            "from " + tableModule + " where " +tableModule+"."+ moduleCol.mac + "="+tableDevices+"."+ col.mac + " "+ facilitiesCond + ")"
//                        ")"
//            ") "
//            "from " + tableDevices +
//                    " where " + col.gateway_id + " in( " << gateways << " ) and " + DAODevices::col.init + "=':init' " 
//                ;
//
//}


string DAODevices::getXMLDevicesQueryString2()
{
    return " xmlagg("
                        "xmlelement(name device,"
                                    "xmlattributes(" + col.init + " as init, " + col.euid + " as id, " + col.id + " as type, " + col.location_id + " as locationid, " + col.refresh + " as refresh, "
                                                " " + col.measured_at + " as time, " + col.involved + "  as involved, " + col.name + "  as name, " + col.gateway_id + " as gateid),"
                                    "(select xmlagg(xmlelement(name module,xmlattributes(" + moduleCol.id + " as id, " + moduleCol.value + " as value))) "
                                            "from " + tableModule + " where " +tableModule+"."+ moduleCol.euid + "="+tableDevices+"."+ col.euid + " )"
                    ")"
                ") "
//            from " + tableDevices + " ";
                ;

}

std::string DAODevices::getXMLAllDevs(long long gateId)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getCommunicationXml"<<"\n";
        
        try
        {
            soci::session sql(*_pool);
            
            std::string xml;
            indicator ind;
            
            sql << "select " + getXMLDevicesQueryString2() +
                    "from  " +  tableDevices + 
                    " where " + col.gateway_id + "=:adapter and " + DAODevices::col.init + "!='0' " 
                    ,use(gateId,"adapter"),soci::into(xml,ind);
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

string DAODevices::getXMLdevices(int userId, vector<long long> gateVector, vector<int> devicesVec)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getPartialDevices"<<"\n";
    try
    {
        soci::session sql(*_pool);

        string xml;
        indicator ind;
        
        if(gateVector.size() == 0 || devicesVec.size() == 0)
            return "";
        
        Logger::debug3()<<"gates:"<<vectorToPsqlNotation(gateVector)<<"\n";
        Logger::debug3()<<"devs:"<<vectorToPsqlNotation(devicesVec)<<"\n";
        
        string gates = vectorToPsqlNotation(gateVector);
        string devices = vectorToPsqlNotation(devicesVec);
        
        statement st = (sql.prepare <<
                
                "select  " <<
                getXMLDevicesQueryString2() +
               "from user_gateway left join gateway using(gateway_id) left join " + tableDevices + " using(gateway_id)" <<
                "where " +col.gateway_id+" in(" + gates + ") and " + col.euid + " in(" + devices + ") and user_id=:userId",
                use(userId),
                soci::into(xml, ind) );
        
        
        
        //cout<<endl<<sql.get_last_query()<<endl;
        st.execute(true);

        return xml;
    }
    catch (postgresql_soci_error& e)
    {
            Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
            throw;
    }
}

string DAODevices::getXMLNewDevices(long long gateId)
{
         Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getPartialDevices"<<"\n";
        try
        {
                soci::session sql(*_pool);
                
                
                string xml;
                indicator ind;
                statement st = (sql.prepare <<"select " +
                        getXMLDevicesQueryString2() +
                       "from " + tableDevices + " where "+col.init+"='0' and " +col.gateway_id+" =:adapter" 
                        ,use(gateId, "adapter"),soci::into(xml, ind) );
                
                st.execute(true);
                             
                return xml;
                
        }
        catch (postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}

int DAODevices::updateFacility(long long gateId, string id, string init, string locationId, string refresh, string name) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update Facility"<<id<<"\n";
    try
    {      
        
        soci::session sql(*_pool);        
        
        string columnsToSet;
        indicator locationInd = i_ok;
        if(locationId == "null")
            locationInd = i_null;
        if(init != "")columnsToSet.append(" "+col.init+"=:init, ");
        if(locationId != "")columnsToSet.append(" "+col.location_id+"=:locId, ");
        if(refresh != "")columnsToSet.append(" "+col.refresh+"=:refresh, ");
        if(name != "")columnsToSet.append(" "+col.name+"=:name, ");
        columnsToSet = columnsToSet.substr(0,columnsToSet.size()-2);//erase last ', '
        
        statement st = (sql.prepare << "update " << tableDevices << " set " <<
                columnsToSet <<
                " where "+col.euid+" = :mac ",
                use(init, "init"),use(locationId, locationInd, "locId"),use(refresh, "refresh"),use(name, "name"),use(id, "mac")
                );
        
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}