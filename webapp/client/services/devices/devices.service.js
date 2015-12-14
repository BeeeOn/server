'use strict';

angular.module('beeeOnWebApp')
  .factory('Devices',['$log','$q','$http','lodash','Gateways','MODULES', function ($log,$q,$http,lodash,Gateways,MODULES) {

    var currentDevices = null;

    var devicesIcons = {
      0 : { "icon" : "b_dev_boiler"},
      1 : { "icon" : "b_dev_boiler"},
      2 : { "icon" : "b_dev_boiler"}
    };
    var devicesTable = {
      "0": {
        "id": "0",
        "typeName": "BeeeOn v1.0",
        "name": "DEVICES__DEV_TEMPERATURE_HUMIDITY",
        "manufacturer": "DEVICES__MANUFACTURER_BUT",
        "modules": [
          {
            "id": "0",
            "type": "0x02",
            "name": "DEVICES__MOD_ROOM_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "1",
            "type": "0x02",
            "name": "DEVICES__MOD_OUTSIDE_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "2",
            "type": "0x03",
            "name": "DEVICES__MOD_ROOM_HUMIDITY",
            "isActuator": false
          },
          {
            "id": "3",
            "type": "0x08",
            "isActuator": false
          },
          {
            "id": "4",
            "type": "0x09",
            "isActuator": false
          },
          {
            "id": "5",
            "type": "0x0A",
            "isActuator": true,
            "constraints": {
              "min" : "5.0",
              "max" : "3600.0",
              "granularity" : "1.0"
            },
            "defaultValue" : "30"
          }
        ]
      },
      "1": {
        "id": "1",
        "typeName": "Regulator VPT v1.0",
        "name": "DEVICES__DEV_REGULATOR_VPT",
        "manufacturer": "DEVICES__MANUFACTURER_THERMONA",
        "modules": [
          {
            "id": "0",
            "type": "0x01",
            "group": "DEVICES__ZONE_1",
            "name": "DEVICES__MOD_BOILER_OPERATION_TYPE",
            "isActuator": true,
            "rules": [
              {
                "value": "0",
                "hiddenIds": [1, 2, 3, 4, 5]
              },
              {
                "value": "1",
                "hiddenIds": [4, 5]
              },
              {
                "value": "2",
                "hiddenIds": [4, 5]
              },
              {
                "value": "3",
                "hiddenIds": [2, 3]
              },
              {
                "value": "4",
                "hiddenIds": [2, 3]
              }
            ],
            "values": {
              "0" : {
                "id": "0",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_OFF"
              },
              "1" : {
                "id": "1",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_ROOM_REGULATOR"
              },
              "2" : {
                "id": "2",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_EQUITERM_REGULATOR"
              },
              "3" : {
                "id": "3",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_CONSTANT_WATER_TEMPERATURE"
              },
              "4" : {
                "id": "4",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_HOT_WATER"
              }
            }
          },
          {
            "id": "1",
            "type": "0x01",
            "group": "DEVICES__ZONE_1",
            "name": "DEVICES__MOD_BOILER_OPERATION_MODE",
            "isActuator": true,
            "values": {
              "0" : {
                "id": "0",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_AUTOMATIC"
              },
              "1" : {
                "id": "1",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_MANUAL"
              },
              "2" : {
                "id": "2",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_VACATION"
              }
            }
          },
          {
            "id": "2",
            "type": "0x02",
            "group": "DEVICES__ZONE_1",
            "name": "DEVICES__MOD_REQUESTED_ROOM_TEMPERATURE",
            "isActuator": true,
            "constraints": {
              "min" : "0.0",
              "max" : "160.0",
              "granularity" : "0.5"
            }
          },
          {
            "id": "3",
            "type": "0x02",
            "group": "DEVICES__ZONE_1",
            "name": "DEVICES__MOD_CURRENT_ROOM_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "4",
            "type": "0x02",
            "group": "DEVICES__ZONE_1",
            "name": "DEVICES__MOD_REQUESTED_WATER_TEMPERATURE",
            "isActuator": true,
            "constraints": {
              "min" : "20.0",
              "max" : "90.0",
              "granularity" : "0.5"
            }
          },
          {
            "id": "5",
            "type": "0x02",
            "group": "DEVICES__ZONE_1",
            "name": "DEVICES__MOD_CURRENT_WATER_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "6",
            "type": "0x01",
            "group": "DEVICES__ZONE_2",
            "name": "DEVICES__MOD_BOILER_OPERATION_TYPE",
            "isActuator": true,
            "rules": [
              {
                "value": "0",
                "hiddenIds": [7, 8, 9, 10, 11]
              },
              {
                "value": "1",
                "hiddenIds": [10, 11]
              },
              {
                "value": "2",
                "hiddenIds": [10, 11]
              },
              {
                "value": "3",
                "hiddenIds": [8, 9]
              },
              {
                "value": "4",
                "hiddenIds": [8, 9]
              }
            ],
            "values": {
              "0" : {
                "id": "0",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_OFF"
              },
              "1" : {
                "id": "1",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_ROOM_REGULATOR"
              },
              "2" : {
                "id": "2",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_EQUITERM_REGULATOR"
              },
              "3" : {
                "id": "3",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_CONSTANT_WATER_TEMPERATURE"
              },
              "4" : {
                "id": "4",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_HOT_WATER"
              }
            }
          },
          {
            "id": "7",
            "type": "0x01",
            "group": "DEVICES__ZONE_2",
            "name": "DEVICES__MOD_BOILER_OPERATION_MODE",
            "isActuator": true,
            "values": {
              "0" : {
                "id": "0",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_AUTOMATIC"
              },
              "1" : {
                "id": "1",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_MANUAL"
              },
              "2" : {
                "id": "2",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_VACATION"
              }
            }
          },
          {
            "id": "8",
            "type": "0x02",
            "group": "DEVICES__ZONE_2",
            "name": "DEVICES__MOD_REQUESTED_ROOM_TEMPERATURE",
            "isActuator": true,
            "constraints": {
              "min" : "0.0",
              "max" : "160.0",
              "granularity" : "0.5"
            }
          },
          {
            "id": "9",
            "type": "0x02",
            "group": "DEVICES__ZONE_2",
            "name": "DEVICES__MOD_CURRENT_ROOM_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "10",
            "type": "0x02",
            "group": "DEVICES__ZONE_2",
            "name": "DEVICES__MOD_REQUESTED_WATER_TEMPERATURE",
            "isActuator": true,
            "constraints": {
              "min" : "20.0",
              "max" : "90.0",
              "granularity" : "0.5"
            }
          },
          {
            "id": "11",
            "type": "0x02",
            "group": "DEVICES__ZONE_2",
            "name": "DEVICES__MOD_CURRENT_WATER_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "12",
            "type": "0x01",
            "group": "DEVICES__ZONE_3",
            "name": "DEVICES__MOD_BOILER_OPERATION_TYPE",
            "isActuator": true,
            "rules": [
              {
                "value": "0",
                "hiddenIds": [13, 14, 15, 16, 17]
              },
              {
                "value": "1",
                "hiddenIds": [16, 17]
              },
              {
                "value": "2",
                "hiddenIds": [16, 17]
              },
              {
                "value": "3",
                "hiddenIds": [14, 15]
              },
              {
                "value": "4",
                "hiddenIds": [14, 15]
              }
            ],
            "values": {
              "0" : {
                "id": "0",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_OFF"
              },
              "1" : {
                "id": "1",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_ROOM_REGULATOR"
              },
              "2" : {
                "id": "2",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_EQUITERM_REGULATOR"
              },
              "3" : {
                "id": "3",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_CONSTANT_WATER_TEMPERATURE"
              },
              "4" : {
                "id": "4",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_HOT_WATER"
              }
            }
          },
          {
            "id": "13",
            "type": "0x01",
            "group": "DEVICES__ZONE_3",
            "name": "DEVICES__MOD_BOILER_OPERATION_MODE",
            "isActuator": true,
            "values": {
              "0" : {
                "id": "0",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_AUTOMATIC"
              },
              "1" : {
                "id": "1",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_MANUAL"
              },
              "2" : {
                "id": "2",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_VACATION"
              }
            }
          },
          {
            "id": "14",
            "type": "0x02",
            "group": "DEVICES__ZONE_3",
            "name": "DEVICES__MOD_REQUESTED_ROOM_TEMPERATURE",
            "isActuator": true,
            "constraints": {
              "min" : "0.0",
              "max" : "160.0",
              "granularity" : "0.5"
            }
          },
          {
            "id": "15",
            "type": "0x02",
            "group": "DEVICES__ZONE_3",
            "name": "DEVICES__MOD_CURRENT_ROOM_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "16",
            "type": "0x02",
            "group": "DEVICES__ZONE_3",
            "name": "DEVICES__MOD_REQUESTED_WATER_TEMPERATURE",
            "isActuator": true,
            "constraints": {
              "min" : "20.0",
              "max" : "90.0",
              "granularity" : "0.5"
            }
          },
          {
            "id": "17",
            "type": "0x02",
            "group": "DEVICES__ZONE_3",
            "name": "DEVICES__MOD_CURRENT_WATER_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "18",
            "type": "0x01",
            "group": "DEVICES__ZONE_4",
            "name": "DEVICES__MOD_BOILER_OPERATION_TYPE",
            "isActuator": true,
            "rules": [
              {
                "value": "0",
                "hiddenIds": [19, 20, 21, 22, 23]
              },
              {
                "value": "1",
                "hiddenIds": [22, 23]
              },
              {
                "value": "2",
                "hiddenIds": [22, 23]
              },
              {
                "value": "3",
                "hiddenIds": [20, 21]
              },
              {
                "value": "4",
                "hiddenIds": [20, 21]
              }
            ],
            "values": {
              "0" : {
                "id": "0",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_OFF"
              },
              "1" : {
                "id": "1",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_ROOM_REGULATOR"
              },
              "2" : {
                "id": "2",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_EQUITERM_REGULATOR"
              },
              "3" : {
                "id": "3",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_CONSTANT_WATER_TEMPERATURE"
              },
              "4" : {
                "id": "4",
                "name": "DEVICES__VAL_BOILER_OPERATION_TYPE_HOT_WATER"
              }
            }
          },
          {
            "id": "19",
            "type": "0x01",
            "group": "DEVICES__ZONE_4",
            "name": "DEVICES__MOD_BOILER_OPERATION_MODE",
            "isActuator": true,
            "values": {
              "0" : {
                "id": "0",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_AUTOMATIC"
              },
              "1" : {
                "id": "1",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_MANUAL"
              },
              "2" : {
                "id": "2",
                "name": "DEVICES__VAL_BOILER_OPERATION_MODE_VACATION"
              }
            }
          },
          {
            "id": "20",
            "type": "0x02",
            "group": "DEVICES__ZONE_4",
            "name": "DEVICES__MOD_REQUESTED_ROOM_TEMPERATURE",
            "isActuator": true,
            "constraints": {
              "min" : "0.0",
              "max" : "160.0",
              "granularity" : "0.5"
            }
          },
          {
            "id": "21",
            "type": "0x02",
            "group": "DEVICES__ZONE_4",
            "name": "DEVICES__MOD_CURRENT_ROOM_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "22",
            "type": "0x02",
            "group": "DEVICES__ZONE_4",
            "name": "DEVICES__MOD_REQUESTED_WATER_TEMPERATURE",
            "isActuator": true,
            "constraints": {
              "min" : "20.0",
              "max" : "90.0",
              "granularity" : "0.5"
            }
          },
          {
            "id": "23",
            "type": "0x02",
            "group": "DEVICES__ZONE_4",
            "name": "DEVICES__MOD_CURRENT_WATER_TEMPERATURE",
            "isActuator": false
          },
          {
            "id": "24",
            "type": "0x01",
            "order": "0",
            "name": "DEVICES__MOD_BOILER_STATUS",
            "isActuator": false,
            "values": {
              "0" : {
                "id": "0",
                "name": "DEVICES__VAL_BOILER_STATUS_UNDEFINED"
              },
              "1" : {
                "id": "1",
                "name": "DEVICES__VAL_BOILER_STATUS_HEATING"
              },
              "2" : {
                "id": "2",
                "name": "DEVICES__VAL_BOILER_STATUS_HOT_WATER"
              },
              "3" : {
                "id": "3",
                "name": "DEVICES__VAL_BOILER_STATUS_FAILURE"
              },
              "4" : {
                "id": "4",
                "name": "DEVICES__VAL_BOILER_STATUS_SHUTDOWN"
              }
            }
          },
          {
            "id": "25",
            "type": "0x09",
            "isActuator": false
          }
        ]
      },
      "2": {
        "id": "2",
        "typeName": "BeeeOn Internal Pressure v1.0",
        "name": "DEVICES__DEV_INTERNAL_PRESSURE",
        "manufacturer": "DEVICES__MANUFACTURER_BUT",
        "modules": [
          {
            "id": "0",
            "type": "0x04",
            "name": "DEVICES__TYPE_PRESSURE",
            "isActuator": false
          },
          {
            "id": "1",
            "type": "0x0A",
            "isActuator": true,
            "constraints": {
              "min" : "5.0",
              "max" : "3600.0",
              "granularity" : "1.0"
            },
            "defaultValue" : "30"
          }
        ]
      }
    };

    /* Public API for Devices service*/
    return {
      getCurrentDevices : function (){
        return currentDevices;
      },
      setCurrentDevices : function (devices){
        currentDevices = devices;
      },
      getDeviceById : function(deviceId){
        return lodash.find(currentDevices,{'id':deviceId});
      },
      getModuleTemplate : function(deviceType,moduleType){
        return lodash.find(devicesTable[deviceType].modules,{'type':moduleType});
      },
      getIconByType : function (type){
        return devicesIcons[type].icon;
      },
      getDefaultNameByType : function (type) {
        return devicesTable[type].name;
      },
      getManufacturerByType : function (type) {
        return devicesTable[type].manufacturer;
      },
      getDeviceBatteryState : function (deviceId){
        if(currentDevices!=null){
          var device = this.getDeviceById(deviceId);
          var moduleTemplate = this.getModuleTemplate(device.type,MODULES.BATTERY);
          if(moduleTemplate){
            return lodash.find(device.modules,{'id':parseInt(moduleTemplate.id)}).value;
          }
        }else{
          $log.warn("Devices - No current devices yet");
        }
        return null;
      },
      hasDeviceBatteryModule : function(type){
        console.log();
        return !angular.equals([],lodash.filter(devicesTable[type].modules,{'type':MODULES.BATTERY}));
      },
      getModuleById : function(deviceType,moduleId){
        var module = lodash.find(devicesTable[deviceType].modules,{'id':moduleId.toString()});
        return module ? module : null;
      },
      getModuleNameByType : function(moduleType){
        return lodash.find(devicesTable)
      },
      setNewModuleState : function(deviceId,module,newValue){
        var deferred = $q.defer();
        $log.debug('Devices - trying set new value (device: ' + deviceId + ',module: ' + module.id + ') to: ' + newValue);
        $http.put('api/v1/gateway/' + Gateways.getSelectedId() + '/devices/'+ deviceId,{moduleId:module.id,value:newValue}).
          then(function(){
            module.value = newValue;
            deferred.resolve();
          },function(err){
            deferred.reject(err);
          });
        return deferred.promise;
      }
    }
  }]);