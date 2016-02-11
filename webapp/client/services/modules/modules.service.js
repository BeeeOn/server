'use strict';

angular.module('beeeOnWebApp')
  .factory('Modules', [ '$log','$translate','Devices','MODULES','lodash', function ($log,$translate,Devices,MODULES,lodash) {

    var emptyUnitConversion = function(value){ return value};
    var secToRefreshInterval = function(value){
      switch(parseInt(value)){
        case 1:
          return value + " " + $translate.instant('GLOBAL.SECOND');
        case 5:
        case 10:
        case 15:
        case 20:
        case 30:
          return value + " " + $translate.instant('GLOBAL.SECONDS');
        case 60:
          return (value / 60) + " " + $translate.instant('GLOBAL.MINUTE');
          break;
        case 60 * 5:
        case 60 * 10:
        case 60 * 15:
        case 60 * 30:
          return (value / 60) + " " + $translate.instant('GLOBAL.MINUTES');
        case 60 * 60:
          return (value / 60 / 60) + " " + $translate.instant('GLOBAL.HOUR');
        case 60 * 60 * 2:
        case 60 * 60 * 3:
        case 60 * 60 * 4:
        case 60 * 60 * 8:
        case 60 * 60 * 12:
        case 60 * 60 * 24:
          return (value / 60 / 60) + " " + $translate.instant('GLOBAL.HOURS');
      }
    };

    var modulesDefaults = {
      "0x01":{
        id : MODULES.ENUM,
        icon : "b_dev_state",
        actuatorIcon : "b_dev_state",
        selectedUnit : 'empty',
        unitOptions : {
          'empty': {id: 'empty', name: '', unit: '',convert: emptyUnitConversion}
        }
      },
      "0x02":{
        id : MODULES.TEMPERATURE,
        icon : "b_dev_temperature",
        actuatorIcon : "b_dev_temperature_actuator",
        selectedUnit : 'celsius',
        unitOptions : {
          'celsius': {id: 'celsius', name: 'SETTINGS.UNITS.TEMPERATURE.CELSIUS', unit: '°C',convert: emptyUnitConversion},
          'fahrenheit': {id: 'fahrenheit', name: 'SETTINGS.UNITS.TEMPERATURE.FAHRENHEIT', unit: '°F',convert: function(value){return (parseFloat(value) * 9 / 5 + 32);}},
          'kelvin': {id: 'kelvin', name: 'SETTINGS.UNITS.TEMPERATURE.KELVIN', unit: 'K',convert: function(value){return parseFloat(value) + 273.15;}}
        }
      },
      "0x03":{
        id : MODULES.HUMIDITY,
        icon : "b_dev_humidity",
        actuatorIcon : "b_dev_humidity",
        selectedUnit : 'percentage',
        unitOptions : {
          'percentage': {id: 'percentage', name: '', unit: '%',convert: emptyUnitConversion}
        }
      },
      "0x04":{
        id : MODULES.PRESSURE,
        icon : "b_dev_pressure",
        actuatorIcon : "b_dev_pressure",
        selectedUnit : 'hpascal',
        unitOptions : {
          'hpascal': {id: 'hpascal', name: '', unit: 'hPa',convert: emptyUnitConversion}
        }
      },
      "0x05":{
        id : MODULES.LIGHT,
        icon : "b_dev_lux",
        actuatorIcon : "b_dev_lux",
        selectedUnit : 'lux',
        unitOptions : {
          'lux': {id: 'lux', name: '', unit: 'lx',convert: emptyUnitConversion}
        }
      },
      "0x06":{
        id : MODULES.NOISE,
        icon : "b_dev_noise",
        actuatorIcon : "b_dev_noise",
        selectedUnit : 'decibel',
        unitOptions : {
          'decibel': {id: 'decibel',name: 'SETTINGS.UNITS.NOISE.DECIBEL', unit: 'dB',convert: emptyUnitConversion},
          'bel': {id: 'bel',name: 'SETTINGS.UNITS.NOISE.BEL', unit: 'B',convert: emptyUnitConversion},
          'neper': {id: 'neper',name: 'SETTINGS.UNITS.NOISE.NEPER', unit: 'Np',convert: emptyUnitConversion}
        }
      },
      "0x07":{
        id : MODULES.EMISSIONS,
        icon : "b_dev_emission",
        actuatorIcon : "b_dev_emission",
        selectedUnit : 'ppm',
        unitOptions : {
          'ppm': {id: 'ppm', name: '', unit: 'ppm',convert: emptyUnitConversion}
        }
      },
      "0x08":{
        id : MODULES.BATTERY,
        icon : "battery_full",
        actuatorIcon : "battery_full",
        selectedUnit : 'percentage',
        unitOptions : {
          'percentage': {id: 'percentage',name: '', unit: '%',convert: emptyUnitConversion}
        }
      },
      "0x09":{
        id : MODULES.RSSI,
        icon : "wifi",
        actuatorIcon : "wifi",
        selectedUnit : 'percentage',
        unitOptions : {
          'percentage': {id: 'percentage', name: '', unit: '%',convert: emptyUnitConversion}
        }
      },
      "0x0A":{
        id : MODULES.REFRESH,
        icon : "autorenew",
        actuatorIcon : "autorenew",
        selectedUnit : 'time',
        unitOptions : {
          'time': {id: 'time', name: '', unit: '',convert: secToRefreshInterval}
        }
      }
    };
    var assignModuleModel = function(deviceType,module){
      module.model = Devices.getModuleById(deviceType,module.id);
      if(module.model){
        //assign icon
        var iconBase = modulesDefaults[module.model.type];
        module.model.isActuator ? module.model.icon = iconBase.actuatorIcon: module.model.icon = iconBase.icon;
        //assign visibility
        module.isVisible = true;
      }
      return module;
    };

    var moduleFindError = function(deviceType,module){
      $log.error("Cannot find model for module or missing information in model (deviceType: " + deviceType + ",moduleId:"+module.id + ")")
    };

    // Public API here
    return {
      getSelectedUnit : function(type){
        return modulesDefaults[type].selectedUnit;
      },
      setSelectedUnit : function(type,unit){
        modulesDefaults[type].selectedUnit = unit;
      },
      getUnitOptions : function(type){
        return modulesDefaults[type].unitOptions;
      },
      getModuleUnitByType: function (type) {
        var module = modulesDefaults[type];
        return module ? module.unitOptions[module.selectedUnit].unit : $log.error("Cannot find moduleDefault for module (type: " + type + ")");
      },
      initModuleModel : function (deviceType,module){
        module = assignModuleModel(deviceType,module);
        return module;
      },
      initAllModuleModels : function (deviceType,modules){
        angular.forEach(modules,function(module){
          if(!module.model){
            module = assignModuleModel(deviceType,module);
          }
        });
        return modules;
      },
      getModuleTypeById : function(deviceType,module){
        module = this.initModuleModel(deviceType,module);
        return (module.model && module.model.type) ? module.model.type : moduleFindError(deviceType,module);
      },
      getModuleIconById: function(deviceType,module){
        module = this.initModuleModel(deviceType,module);
        return (module.model && module.model.icon ) ? module.model.icon : moduleFindError(deviceType,module);
      },
      getModuleNameById : function(deviceType,module){
        module = this.initModuleModel(deviceType,module);
        return (module.model && module.model.name) ? module.model.name : null;
      },
      getConvertedValue : function(deviceType,module){
        module = this.initModuleModel(deviceType,module);
        var tmp = modulesDefaults[module.model.type];
        return (module && module.model) ? tmp.unitOptions[tmp.selectedUnit].convert(module.value) : moduleFindError(deviceType,module);
      },
      getModuleUnit : function(deviceType,module){
        module = this.initModuleModel(deviceType,module);
        var tmp = modulesDefaults[module.model.type];
        return (module && module.model) ? tmp.unitOptions[tmp.selectedUnit].unit : moduleFindError(deviceType,module);
      },
      getModuleEnumValueName : function(deviceType,module){
        module = this.initModuleModel(deviceType,module);
        return (module.model && module.model.values) ? module.model.values[module.value].name : moduleFindError(deviceType,module);
      },
      getModulesToBeHiddenByRule : function(deviceType,module){
        module = this.initModuleModel(deviceType,module);
        if(!module.model.rules){
          return [];
        }else{
          return lodash.find(module.model.rules,{'value':module.value}).hiddenIds;
        }
      }
    };
  }]);
