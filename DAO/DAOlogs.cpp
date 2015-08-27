/* 
 * File:   DAOlogs.cpp
 * Author: pavel
 * 
 * Created on 29. červenec 2015, 11:07
 */

#include "DAOlogs.h"
#include "msgs/LocationAdd.h"
#include "../ui_logger/Logger.h"
#include "ServerException.h"
using namespace std;
using namespace soci;

const LogsColumns  DAOlogs::col;
const string DAOlogs::tableLogs = "log";

DAOlogs::DAOlogs() {
}

DAOlogs::~DAOlogs() {
}


DAOlogs& DAOlogs::getInstance(){
        static DAOlogs instance;
        return instance;
}




string DAOlogs::getXMLDeviceLog(long long adapterId, device device, string logFrom, string logTo, string dataType, string interval)
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
            
            timeCond =  col.time + " between "+escapeString(logFrom)+" and "+escapeString(logTo)+" ";

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
                        " xmlelement(name row, " << col.time << "  || ' ' || " << col.value << " )"
                        ")"
                        "from " << tableLogs << " where  " << col.device_euid << " =:d_id and  " << col.module_id << "= :d_type and "+ timeCond
                        ,use(device.id,"d_id"), use(device.type, "d_type"), soci::into(xml, ind);
            }else{
                Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"data by "<< DBIntervalString <<" and " << timeCond <<"\n";
                
                //cout<<"SELECT * FROM generate_series('"+logFrom+"'::timestamp,'"+logTo+"', '1 "+DBIntervalString+"') as full_time; "<<endl;
                //cout<<"select date_trunc('"+DBIntervalString+"',max(timestamp) ), avg(value) from logs where fk_devices_mac= '"+device.id+"' and  fk_devices_type= '"+device.type+"'"+ "group by date_trunc('"+DBIntervalString+"',timestamp )  ;"<<endl;
                ostringstream oss ;
               /* oss<<"select xmlagg( xmlelement(name row, to_char(full_time,xxxxxxxx) ,' ' ,round( coalesce(avg, 'NaN') , 2 )) ) from"
                        "(select * from generate_series('"+logFrom+"'::timestamp,'"+logTo+"', '1 "+DBIntervalString+"') as full_time) t1 "
                        "left join "
                        "(select date_trunc('"+DBIntervalString+"',max(logs.timestamp) ), "+aggregationFunction+"(logs.value) from logs where logs.fk_facilities_mac=:d_id and  fk_devices_type= :d_type "
                        "and "+timeCond+//" and (select mac from " + deviceTname + " where fk_adapter_id=:a_id and mac=:d_id and type=:d_type) is not null "
                        "group by date_trunc('"+DBIntervalString+"',logs.timestamp )  order by date_trunc('"+DBIntervalString+"',max(logs.timestamp))  ) t2 "
                        "on full_time=date_trunc";*/
                 //TODO nezavisi na adapter_id        
                
                oss<<"select xmlagg( xmlelement(name row,a,' ',b ))from (select ceil(avg(" << col.time << ")) as a,  trunc("<<aggregationFunction<<"(" << col.value << ")::numeric,2) as b from "
                         << tableLogs << " where  " << col.device_euid << "= :d_id and  " << col.module_id << "= :d_type and "<< timeCond<<" group by " << col.time << "/"<<intInterval<<") as innerQ";
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