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
const string InsertGateway = "insert into gateway (gateway_id,version,socket) values (:GATEWAY_ID,:FM_VERSION,:SOCKET);";  //"insert into adapters (adapter_id,version,socket) values ( "+ std::to_string(message->adapterINTid)+ ", " + std::to_string(message->fm_version) + ", '" + std::to_string(message->socket) + "');" ;
/** Q to update adapter uses ( :AdapterID, :FMver)*/
const string UpdateGateway = "update gateway set version = :FM_VERSION where gateway_id = :GATEWAY_ID;"; //"update adapters set version=" + std::to_string(message->fm_version) + ", socket=" + std::to_string(message->socket) + " where adapter_id=" + std::to_string(message->adapterINTid) + ";" ;
/** Q to update socket of adapter uses ( :AdapterID, :FMver, :socket)*/
const string UpdateGatewaySocket = "update gateway set version = :FM_VERSION, socket = :SOCKET where gateway_id = :GATEWAY_ID;"; //"update adapters set version=" + std::to_string(message->fm_version) + ", socket=" + std::to_string(message->socket) + " where adapter_id=" + std::to_string(message->adapterINTid) + ";" ;
/** Q to select socket of adapter uses (:ID) */
const string SelectSocket = "SELECT socket FROM gateway where gateway_id=:ID;";

/**Q to insert facility uses ( ':deviceID',:battery, :signal, :adapterID, :timestamp , :timestamp)*/
const string InsertDevice = "insert into device (device_type,device_euid,refresh,gateway_id,involved,measured_at) values (:DEVICE_TYPE,:DEVICE_EUID,15,:GATEWAY_ID,:TIMESTAMP,:TIMESTAMP);";  /*"insert into facilities (mac,refresh,battery,quality,fk_adapter_id,involved,timestamp) values
( '" + message->DeviceIDstr + "', 5 ," + std::to_string(message->battery)+ ", " + std::to_string(message->signal_strength) +  ", " +std::to_string(message->adapterINTid)+ ", " +std::to_string(message->timestamp) + ", " + std::to_string(message->timestamp) +" );" ;*/
/** Q to update facility uses ( :battery, :quality, :timestamp, :mac)*/
const string UpdateDevice = "update device set measured_at = :MEASURED_AT where (device_euid = :DEVICE_EUID) AND gateway_id = :GATEWAY_ID;"; //"update facilities set battery=" + std::to_string(message->battery) + ",quality=" + std::to_string(message->signal_strength) + ",timestamp=" + std::to_string(message->timestamp)  +" where (mac='" + message->DeviceIDstr + "');" ;
/** Q to Delete facility uses (:ID) */
const string DeleteDevice = "Delete from device where device_euid=:DEVICE_EUID AND gateway_id=_GATEWAY_ID;";
/** Q to select wake up time of record uses ( :record)*/
const string SelectTime = "select refresh from device where device_euid=:record AND gateway_id=:GATEWAY_ID;"; //"select refresh from facilities where mac = '" + record + "';";

/**Q to insert device uses ( ':devID', :type, ':value')*/
const string InsertModule = "insert into module (device_euid,module_id,measured_value, gateway_id) values (:DEVICE_EUID,:MODULE_ID,:MEASURED_VALUE, :GATEWAY_ID);";  //"insert into devices (fk_facilities_mac,type,value) values ( '" + message->DeviceIDstr + "', " + std::to_string(message->values[i].intType) + ", '" + val + "');" ;
/** Q to update device  uses ( :value, :FMver, ':mac', :type)*/
const string UpdateModule = "update module set measured_value = :MEASURED_VALUE where (device_euid = :DEVICE_EUID AND module_id = :MODULE_ID AND gateway_id=:GATEWAY_ID);";  //"update devices set value=" + val  +" where (fk_facilities_mac='" + message->DeviceIDstr + "' AND type =" + std::to_string(message->values[i].intType) +");" ;

/** Q to insert history row uses ( ':deviceID', :timeStamp , :type, :value)*/
const string InsertLog = "insert into log (device_euid,measured_at,module_id,measured_value, gateway_id) values (:DEVICE_EUID,:MEASURED_AT,:MODULE_ID,:MEASURED_VALUE, :GATEWAY_ID);"; //"insert into logs (fk_facilities_mac,timestamp,fk_devices_type,value) values ( '"+ message->DeviceIDstr + "', " + std::to_string(message->timestamp) + " , " + std::to_string(message->values[i].intType) + ", " + val + " );" ;

/** Q to select count of record uses ( :coulmnName :tableName :coumnName :record)*/
const string SelectCount = "select count(*) from :tableName where :columnName = :record;"; //"select count(*)" + columnName + " from " + tableName + " where " + columnName + " = "+ record + ";";
}
#endif /* SQLCOMMANDS_H_ */