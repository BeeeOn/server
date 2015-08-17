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

string DAODevices::getXMLDevicesQueryString(string facilitiesCond)
{
    return " xmlagg("
                        "xmlelement(name  dev,xmlattributes(" + col.init + " as init, " + col.mac + " as did, " + col.id + " as type, " + col.location_id + " as lid, " + col.refresh + " as refresh, "
                                                " " + col.measured_at + " as time, " + col.involved + "  as inv, " + col.name + "  as name),"
                                    "(select xmlagg(xmlelement(name part,xmlattributes(" + moduleCol.id + " as type, " + moduleCol.value + " as val))) "
                                            "from " + tableModule + " where " +tableModule+"."+ moduleCol.mac + "="+tableDevices+"."+ col.mac + " "+ facilitiesCond + ")"
                    ")"
                ") "
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
            
            sql << "select " + getXMLDevicesQueryString() +
                    "from  " + DAOAdapters::tableGateway + " join " + tableDevices + " using( " + DAOAdapters::col.id + ") "
                    " where " + DAOAdapters::col.id + "=:adapter and " + DAODevices::col.init + "!='0' " 
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
        Logger::debug3()<<"gates:"<<vectorToPsqlNotation(gateVector)<<"\n";
        Logger::debug3()<<"devs:"<<vectorToPsqlNotation(devicesVec)<<"\n";
        
        string gates = vectorToPsqlNotation(gateVector);
        string devices = vectorToPsqlNotation(devicesVec);
        
        statement st = (sql.prepare <<
                
                "select  xmlagg("
		"xmlelement(name adapter, xmlattributes(" + DAOAdapters::col.id + " as id),"
					    "(select  "
                <<
                getXMLDevicesQueryString(" and " + col.mac + " in(" + devices + ") " ) +
               "from " + tableDevices + " " <<
                "where " +col.gateway_id+" in(" + gates + ") and " + col.mac + " in(" + devices + ") and " <<
                 DAOAdapters::tableGateway << "." << col.gateway_id << "=" << tableDevices << "." << col.gateway_id <<
                "))) from gateway left join user_gateway using(gateway_id) where user_id = :user and " +col.gateway_id+" in(" + gates + ") ",
                //use(gates,"gates"),use(devices,"devices"),
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

string DAODevices::getXMLdevices(int userId, vector<string> gateVector, vector<device> devicesVec)
{
         Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getPartialDevices"<<"\n";
        try
        {
                soci::session sql(*_pool);
                Logger::getInstance(Logger::DEBUG3)<<"devices n."<<devicesVec.size()<<"\n";
                if( devicesVec.size() == 0)
                    return "";
                
                stringstream ssD,ssF,ssA;
                ssD<<"(";
                 ssF<<"(";
                for (unsigned int i=0; i<devicesVec.size(); i++){
                    ssD << "("<<devicesVec[i].id<<","<<devicesVec[i].type<<")";
                    ssF << ""<<devicesVec[i].id<<"";
                    
                    if( i != devicesVec.size()-1){
                        ssD <<",";
                        ssF <<",";
                    }
                }
                ssD<<")";
                ssF<<")";
                 
                ssA<<"(";
                for (unsigned int i=0; i<gateVector.size(); i++){
                    ssA << "'"<<gateVector[i]<<"'";
                    
                    if( i != gateVector.size()-1){
                        ssA <<",";
                    }
                }
                ssA<<")";
                string devices = ssD.str();
                string facs = ssF.str();
                string adapters = ssA.str();
              
                Logger::getInstance(Logger::DEBUG3)<<"check ada string::"<<adapters<<endl;
                Logger::getInstance(Logger::DEBUG3)<<"check devices string:"<<devices<<endl;
                Logger::getInstance(Logger::DEBUG3)<<"check facilities string::"<<facs<<endl;
                
                string xml;
                indicator ind;
                //cout<<getXMLDevicesQueryString(" and concat(fk_facilities_mac,type) in "+devices);
                statement st = (sql.prepare <<
                        "select  xmlagg("
		"xmlelement(name adapter, xmlattributes(" + DAOAdapters::col.id + " as id),"
					    "(select  "
						    "xmlagg(xmlelement(name  dev,xmlattributes(" + col.mac + " as did, " + col.location_id + " as lid, " + col.refresh + " as refresh, "
                                                                                                                                     " " + col.measured_at + " as time, " + col.involved + "  as inv),"
								"(select xmlagg(xmlelement(name part,xmlattributes(" + moduleCol.id + " as type, " + moduleCol.value + " as val))) from " + tableModule + " where "
									"("+moduleCol.mac+" , "+col.id+") in "+devices+" "
									"and " + tableModule + "."+moduleCol.mac+" = " + tableDevices + "." +col.mac+
								")"								
							   ")"
						   " ) from " + tableDevices + " where " + tableDevices + "."+col.mac+" in "+facs +" and "
						   +" "+DAOAdapters::tableGateway+"."+DAOAdapters::col.id+ "=" + tableDevices + "."+col.gateway_id +
                        
						")"
		")"
	") "
"from " + DAOAdapters::tableGateway + " where "+DAOAdapters::col.id+" in "+adapters
                        ,
                        soci::into(xml, ind) );
                //TODO napojeni na uživatlee
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
                        getXMLDevicesQueryString() +
                       "from " + DAOAdapters::tableGateway + " join " + tableDevices + " using(" +col.gateway_id+") where "+col.init+"='0' and " +col.gateway_id+" =:adapter" //join devices on mac=fk_facilities_mac
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
        if(init != "")columnsToSet.append(" "+col.init+"=:init', ");
        if(locationId != "")columnsToSet.append(" "+col.location_id+"=:locId, ");
        if(refresh != "")columnsToSet.append(" "+col.refresh+"=:refresh, ");
        if(name != "")columnsToSet.append(" "+col.name+"=:name, ");
        columnsToSet = columnsToSet.substr(0,columnsToSet.size()-2);//erase last ', '
        
        statement st = (sql.prepare << "update " << tableDevices << " set " <<
                columnsToSet <<
                " where "+col.mac+" = :mac ",
                use(init, "init"),use(locationId, "locId"),use(refresh, "refresh"),use(name, "name"),use(id, "mac")
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