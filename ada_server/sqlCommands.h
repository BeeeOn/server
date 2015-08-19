/**
 * @file sqlCommands.h
 * 
 * @brief definition of namespace SQLQueries
 *
 * @author Matus Blaho 
 * @version 1.0
 */

#ifndef SQLCOMMANDS_H_
#define SQLCOMMANDS_H_
#include <string>

/** @namespace SQLQueries
 *  @brief groups SQL queries to database used by application
 */

namespace SQLQueries
{

using namespace std;
/**Q to insert adapter uses ( :AdapterID, :FMver, ':socket')*/
const string InsertAdapter = "insert into gateway (gateway_id,version,socket) values (:AdapterID,:FMver,:socket);";  //"insert into adapters (adapter_id,version,socket) values ( "+ std::to_string(message->adapterINTid)+ ", " + std::to_string(message->fm_version) + ", '" + std::to_string(message->socket) + "');" ;
/**Q to insert device uses ( ':devID', :type, ':value')*/
const string InsertDevice = "insert into module (device_mac,module_id,measured_value) values (:devID,:type,:value);";  //"insert into devices (fk_facilities_mac,type,value) values ( '" + message->DeviceIDstr + "', " + std::to_string(message->values[i].intType) + ", '" + val + "');" ;
/**Q to insert facility uses ( ':deviceID',:battery, :signal, :adapterID, :timestamp , :timestamp)*/
const string InsertFacility = "insert into device (device_mac,refresh,battery,quality,gateway_id,involved,measured_at) values (:deviceID,5,:battery,:signal,:adapterID,:timestamp,:timestamp);";  /*"insert into facilities (mac,refresh,battery,quality,fk_adapter_id,involved,timestamp) values
( '" + message->DeviceIDstr + "', 5 ," + std::to_string(message->battery)+ ", " + std::to_string(message->signal_strength) +  ", " +std::to_string(message->adapterINTid)+ ", " +std::to_string(message->timestamp) + ", " + std::to_string(message->timestamp) +" );" ;*/
/** Q to update device  uses ( :value, :FMver, ':mac', :type)*/
const string UpdateDevice = "update module set measured_value = :value where (device_mac = :deviceID AND module_id = :type);";  //"update devices set value=" + val  +" where (fk_facilities_mac='" + message->DeviceIDstr + "' AND type =" + std::to_string(message->values[i].intType) +");" ;
/** Q to update facility uses ( :battery, :quality, :timestamp, :mac)*/
const string UpdateFacility = "update device set battery = :battery, quality = :quality, measured_at = :timestamp where (device_mac = :mac);"; //"update facilities set battery=" + std::to_string(message->battery) + ",quality=" + std::to_string(message->signal_strength) + ",timestamp=" + std::to_string(message->timestamp)  +" where (mac='" + message->DeviceIDstr + "');" ;
/** Q to update adapter uses ( :AdapterID, :FMver)*/
const string UpdateAdapter = "update gateway set version = :FMver where gateway_id = :ID;"; //"update adapters set version=" + std::to_string(message->fm_version) + ", socket=" + std::to_string(message->socket) + " where adapter_id=" + std::to_string(message->adapterINTid) + ";" ;
/** Q to update socket of adapter uses ( :AdapterID, :FMver, :socket)*/
const string UpdateAdapterSocket = "update gateway set version = :FMver, socket = :socket where gateway_id = :ID;"; //"update adapters set version=" + std::to_string(message->fm_version) + ", socket=" + std::to_string(message->socket) + " where adapter_id=" + std::to_string(message->adapterINTid) + ";" ;
/** Q to select count of record uses ( :coulmnName :tableName :coumnName :record)*/
const string SelectCount = "select count(*) from :tableName where :columnName = :record;"; //"select count(*)" + columnName + " from " + tableName + " where " + columnName + " = "+ record + ";";
/** Q to insert history row uses ( ':deviceID', :timeStamp , :type, :value)*/
const string InsertLog = "insert into log (device_mac,measured_at,module_id,measured_value) values (:deviceID,:timeStamp,:type,:value);"; //"insert into logs (fk_facilities_mac,timestamp,fk_devices_type,value) values ( '"+ message->DeviceIDstr + "', " + std::to_string(message->timestamp) + " , " + std::to_string(message->values[i].intType) + ", " + val + " );" ;
/** Q to select wake up time of record uses ( :record)*/
const string SelectTime = "select refresh from device where device_mac=:record;"; //"select refresh from facilities where mac = '" + record + "';";
/** Q to select socket of adapter uses (:ID) */
const string SelectSocket = "SELECT socket FROM gateway where gateway_id=:ID;";
/** Q to Delete facility uses (:ID) */
const string DeleteDevice = "Delete from device where device_mac=:ID;";
//hello
}
#endif /* SQLCOMMANDS_H_ */
