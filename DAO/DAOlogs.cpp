#include "DAOlogs.h"
#include "request_handlers/LocationsAddRequestHandler.h"
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

string DAOlogs::getXMLDeviceLog(gatewayId_64b gateId, device device, long long logFrom, long long logTo, string aggregFunction, long long interval)
{
    Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"log (dev="<<device.id<<")\n";
    
    try{
            soci::session sql(*_pool);

            string aggregationFunction;
            if(aggregFunction == "avg")
                aggregationFunction = "avg";
            else if(aggregFunction == "min")
                aggregationFunction = "min";
            else if(aggregFunction == "max")
                aggregationFunction = "max";
            else
                throw ServerException(ServerException::AGG_FUNCTION);
            
            string xml;
            indicator ind;
            if( interval == 0){ //RAW output
                Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"RAW data\n";
                
                sql<<"select xmlagg("
                        " xmlelement(name row, " << col.time << "  || ' ' || " << col.value << " || chr(10) )"
                        ")"
                        "from " << tableLogs << " where  " << col.device_euid << " =:dId AND  " << col.module_id << "= :dType AND " << col.gateway_id << "= :gateId AND " 
                        << col.time << " between :from and :to "
                        ,use(device.id,"dId"), use(device.type, "dType"), use(gateId, "gateId"),
                        use(logFrom, "from"), use(logTo, "to"),
                        soci::into(xml, ind);
            }else{// aggregate output
                Logger::getInstance(Logger::DEBUG3)<<"DB:"<<"aggreated data\n";
                
                ostringstream oss ;
                // regex '([^.]*[.]?[0-9]{0,2}).*' in SUBSTRING is for truncate float to 2 decimal places, instead of trunc(value::numeric, 2) this implementation preserves values like Infinity etc.
                oss<<"select xmlagg( xmlelement(name row,a,' ',b ))from (select ceil(avg(" << col.time << ")) as a, SUBSTRING("<<aggregationFunction<<"(" << col.value << ")::text, '([^.]*[.]?[0-9]{0,2}).*') as b from "
                         << tableLogs << " where  " << col.device_euid << "= :dId AND  " << col.module_id << "= :dType AND " << col.gateway_id << "= :gateId AND "
                         << col.time << " between :logFrom and :logTo group by " << col.time << "/"<<interval<<") as innerQ";
                string query;
                query = oss.str();
                
                Logger::getInstance(Logger::DEBUG3)<<query<<endl;
                sql << query, use(gateId,"gateId"), use(device.id,"dId"), use(device.type, "dType"), 
                        use(logFrom, "logFrom"), use(logTo, "logTo"),
                        soci::into(xml, ind);
                
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