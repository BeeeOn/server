/* 
 * File:   DAODevices.cpp
 * Author: pavel
 * 
 * Created on 27. březen 2015, 13:33
 */

#include "DAODevices.h"
#include "ServerException.h"
#include <postgresql/soci-postgresql.h>
using namespace std;
using namespace soci;

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
                        "xmlelement(name  dev,xmlattributes(init as init, mac as did, fk_room_id as lid, refresh as refresh, battery as batt,"
                                                " timestamp as time, involved  as inv, quality as rssi),"
                                                 "(select xmlagg(xmlelement(name part,xmlattributes(type as type, visibility as vis, name as name, value as val)))from devices where fk_facilities_mac=mac "+facilitiesCond+")"
                    ")"
                ") "
                ;

}
std::string DAODevices::getXMLAllDevs(string adapter)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"getCommunicationXml"<<"\n";
        
        try
        {
            soci::session sql(*_pool);
            
            std::string xml;
            indicator ind;
            
            sql << "select " +
                                           getXMLDevicesQueryString() +
                    "from  adapters  join facilities on adapters.adapter_id=facilities.fk_adapter_id"
                    " where adapters.adapter_id=:adapter and init!='0' " 
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
		"xmlelement(name adapter, xmlattributes(adapter_id as id),"
					    "(select  "
						    "xmlagg(xmlelement(name  dev,xmlattributes(mac as did, fk_room_id as lid, refresh as refresh, battery as batt,"
                                                                                                                                     " timestamp as time, involved  as inv, quality as rssi),"
								"(select xmlagg(xmlelement(name part,xmlattributes(type as type, visibility as vis, name as name,value as val))) from devices where "
									"(fk_facilities_mac , type) in "+devices+" "
									"and devices.fk_facilities_mac = facilities.mac"
								")"								
							   ")"
						   " ) from facilities where facilities.mac in "+facs +" and"
						   " adapters.adapter_id=facilities.fk_adapter_id"
							
						")"
		")"
	") "
"from adapters where adapter_id in "+adapters
                        ,
                        soci::into(xml, ind) );
                //TODO napojeni na uživatlee
                st.execute(true);
                return xml;
                /*
              
                string searchedString;
                string errText;
                
                for (unsigned int i=0; i<devices.size(); i++){
                    
                    searchedString = devices[i].id + "\" " + "type=\"" + devices[i].type;
                    if(xml.find( searchedString ) ==std::string::npos)
                        errText += "<device id=\"" + devices[i].id + "\" type=\"" + devices[i].type + "\" />";
                }
 * //TODO pozdeji udělat kontroly
 *  //TODO castecny neuspech znamena false
  */
               /*
                if(errText == "")
                    return xml;
                else
                    throw ServerException(ServerException::DEVICE_ID_TYPE, errText);
                */
        }
        catch (postgresql_soci_error& e)
        {
                Logger::getInstance(Logger::ERROR) << "Error: " << e.what() << '\n';
                throw;
        }
}


string DAODevices::getXMLNewDevices(string adapterId)
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
                       "from adapters join facilities on adapter_id=fk_adapter_id  where init='0' and adapter_id =:adapter" //join devices on mac=fk_facilities_mac
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

int DAODevices::updateDevice(string adapterId, string id, string type, string name, string visibility)
{
        Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"updateDevice"<<id<<"\n";
        try
        {       
                soci::session sql(*_pool);
                string query  = "update devices set";
                if(name != "")query.append(" name='"+escapeString(name)+"', ");
                if(visibility != "")query.append(" visibility='"+escapeString(visibility)+"', ");
                
                query = query.substr(0,query.size()-2);//erase last ', '
                
                query.append(" where fk_facilities_mac ='"+escapeString(id)+"' and type='"+escapeString(type)+"' ");
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


string DAODevices::getXMLDeviceLog(string adapterId, device device, string logFrom, string logTo, string dataType, string interval)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"log (dev="<<device.id<<")\n";
    
    try{
            soci::session sql(*_pool);

            string timeCond;
            
            if(logFrom == "")
                logFrom = "0";
            if(logTo == "")
                logTo = "9223372036854775807";//max bigint
            
            string aggregationFunction;
            if(dataType == "avg")
                aggregationFunction = "avg";
            else if(dataType == "min")
                aggregationFunction = "min";
            else if(dataType == "max")
                aggregationFunction = "max";
            else
                throw ServerException(ServerException::AGG_FUNCTION);
            
            timeCond = " logs.timestamp between "+escapeString(logFrom)+" and "+escapeString(logTo)+" ";

            string DBIntervalString;
             int intInterval;
            try{
                intInterval = atoi( interval.c_str() );
            }
            catch(...){
                intInterval=0;
            }
            string xml;
            indicator ind;
            if( intInterval==0){ //RAW output
                Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"RAW data\n";
                
                sql<<"select xmlagg("
                        " xmlelement(name row, timestamp || ' ' || logs.value )"
                        ")"
                        "from logs where  fk_facilities_mac= :d_id and  fk_devices_type= :d_type and"+ timeCond
                        ,use(device.id,"d_id"), use(device.type, "d_type"), soci::into(xml, ind);
            }else{
                Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"data by "<< DBIntervalString <<" and" << timeCond <<"\n";
                
                //cout<<"SELECT * FROM generate_series('"+logFrom+"'::timestamp,'"+logTo+"', '1 "+DBIntervalString+"') as full_time; "<<endl;
                //cout<<"select date_trunc('"+DBIntervalString+"',max(timestamp) ), avg(value) from logs where fk_devices_mac= '"+device.id+"' and  fk_devices_type= '"+device.type+"'"+ "group by date_trunc('"+DBIntervalString+"',timestamp )  ;"<<endl;
                ostringstream oss ;
               /* oss<<"select xmlagg( xmlelement(name row, to_char(full_time,xxxxxxxx) ,' ' ,round( coalesce(avg, 'NaN') , 2 )) ) from"
                        "(select * from generate_series('"+logFrom+"'::timestamp,'"+logTo+"', '1 "+DBIntervalString+"') as full_time) t1 "
                        "left join "
                        "(select date_trunc('"+DBIntervalString+"',max(logs.timestamp) ), "+aggregationFunction+"(logs.value) from logs where logs.fk_facilities_mac=:d_id and  fk_devices_type= :d_type "
                        "and "+timeCond+//" and (select mac from devices where fk_adapter_id=:a_id and mac=:d_id and type=:d_type) is not null "
                        "group by date_trunc('"+DBIntervalString+"',logs.timestamp )  order by date_trunc('"+DBIntervalString+"',max(logs.timestamp))  ) t2 "
                        "on full_time=date_trunc";*/
                 //TODO nezavisi na adapter_id        
                
                oss<<"select xmlagg( xmlelement(name row,a,' ',b ))from (select ceil(avg(timestamp)) as a,  trunc("<<aggregationFunction<<"(value)::numeric,2) as b from "
                        "logs where  fk_facilities_mac= :d_id and  fk_devices_type= :d_type and"<< timeCond<<" group by timestamp/"<<intInterval<<") as innerQ";
                        //select xmlagg(xmlelement(name row,a,' ',b ))from (select ceil(avg(timestamp)) as a,min(value) as b from logs where timestamp between 0 and 10000000000 group by timestamp/2) as x;
                string query;
                query= oss.str();
                Logger::getInstance(Logger::DEBUG3)<<query<<endl;
                sql << query,use(adapterId,"a_id"), use(device.id,"d_id"), use(device.type, "d_type"), soci::into(xml, ind);
                
            }
            
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

string DAODevices::escapeString(string str) {
    char escaped[300];
    PQescapeString(escaped, str.c_str(), 300);
    return escaped;
}


int DAODevices::updateFacility(string adapterId, string id, string init, string locationId, string refresh) {
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"update Facility"<<id<<"\n";
    try
    {      
        
        soci::session sql(*_pool);
        string query = "update facilities set";
        if(init != "")query.append(" init='"+escapeString(init)+"', ");
        if(locationId != "")query.append(" fk_room_id="+escapeString(locationId)+", ");
        if(refresh != "")query.append(" refresh='"+escapeString(refresh)+"', ");
        
        query = query.substr(0,query.size()-2);//erase last ', '
        query.append(" where mac ='"+escapeString(id)+"' ");
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