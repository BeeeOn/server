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



string DAODevices::getXMLDevicesQueryString(string facilitiesCond)
{
    //TODO inv as involved tam nemá být pokud init = 1
    return " xmlagg("
                        "xmlelement(name  dev,xmlattributes(" + col.init + " as init, " + col.mac + " as did, " + col.location_id + " as lid, " + col.refresh + " as refresh, "
                                                " " + col.measured_at + " as time, " + col.involved + "  as inv),"
                                                 "(select xmlagg(xmlelement(name part,xmlattributes(" + moduleCol.id + " as type, " + moduleCol.value + " as val)))from " + tableModule + " where " + moduleCol.mac + "="+ col.mac + " "+ facilitiesCond + ")"
                    ")"
                ") "
                ;

}
std::string DAODevices::getXMLAllDevs(long long adapter)
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
                    ,use(adapter,"adapter"),soci::into(xml,ind);
            

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

string DAODevices::getXMLdevices(int userId, vector<string> adaptersVec, vector<device> devicesVec)
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
                for (unsigned int i=0; i<adaptersVec.size(); i++){
                    ssA << "'"<<adaptersVec[i]<<"'";
                    
                    if( i != adaptersVec.size()-1){
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


string DAODevices::getXMLNewDevices(long long adapterId)
{
         Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getPartialDevices"<<"\n";
        try
        {
                soci::session sql(*_pool);
                
                
                string xml;
                indicator ind;
//TODO možná tady nemusi byt join na devices
                statement st = (sql.prepare <<"select " +
                        getXMLDevicesQueryString() +
                       "from " + DAOAdapters::tableGateway + " join " + tableDevices + " using(" +col.gateway_id+") where "+col.init+"='0' and " +col.gateway_id+" =:adapter" //join devices on mac=fk_facilities_mac
                        ,use(adapterId, "adapter"),soci::into(xml, ind) );
                
                st.execute(true);
                             
                return xml;
                
        }
        catch (postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}

int DAODevices::updateDevice(long long adapterId, string id, string type, string name, string visibility)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"updateDevice"<<id<<"\n";
        try
        {       
                soci::session sql(*_pool);
                string query  = "update " + tableModule + " set ";
                if(name != "")query.append(col.name +"='"+escapeString(name)+"', ");
                
                query = query.substr(0,query.size()-2);//erase last ', '
                
                query.append(" where "+moduleCol.mac+"='"+escapeString(id)+"' and "+moduleCol.id+"='"+escapeString(type)+"' ");
                //and fk_adapter_id='"+escapeString(adapterId)+"'");
                
                Logger::getInstance(Logger::DEBUG3)<<"SQL partial update: "<<query<<"\n";
                
                statement st = (sql.prepare << query);
                st.execute(true);
                
                return st.get_affected_rows();
                                            
        }
        catch (soci::postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                return 0;
        }
}





int DAODevices::updateFacility(long long adapterId, string id, string init, string locationId, string refresh) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update Facility"<<id<<"\n";
    try
    {      
        
        soci::session sql(*_pool);
        string query = "update " + tableDevices + " set";
        if(init != "")query.append(" "+col.init+"='"+escapeString(init)+"', ");
        if(locationId != "")query.append(" "+col.location_id+"="+escapeString(locationId)+", ");
        if(refresh != "")query.append(" "+col.refresh+"='"+escapeString(refresh)+"', ");
        
        query = query.substr(0,query.size()-2);//erase last ', '
        query.append(" where "+col.mac+" ='"+escapeString(id)+"' ");
        //and fk_adapter_id='"+escapeString(adapterId)+"'");
        
        statement st = (sql.prepare << query);
        st.execute(true);
        return st.get_affected_rows();
    }
    catch (soci::postgresql_soci_error& e)
    {
        Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
        return 0;
    }
}