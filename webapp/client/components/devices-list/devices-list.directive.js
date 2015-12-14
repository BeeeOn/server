'use strict';

angular.module('beeeOnWebApp')
  .directive('devicesList',['$state','$stateParams','$log','$translate','lodash','SidePanel','Devices','Modules','Locations', function ($state,$stateParams,$log,$translate,lodash,SidePanel,Devices,Modules,Locations) {
    return {
      templateUrl: 'components/devices-list/devices-list.html',
      restrict: 'E',
      scope : {
        locations : '=',
        devices : '='
      },
      link: function (scope, element, attrs) {

        scope.getIcon = function(type){
          return Devices.getIconByType(type);
        };

        scope.getDefaultDeviceName = function(type){
          return Devices.getDefaultNameByType(type);
        };

        scope.getManufacturer = function(type){
          return Devices.getManufacturerByType(type);
        };

        scope.showBatteryState = function(type){
          return Devices.hasDeviceBatteryModule(type);
        };

        scope.getBatteryState = function(deviceId){
          return Devices.getDeviceBatteryState(deviceId);
        };

        scope.getModuleUnit = function(moduleType){
          return Modules.getModuleUnitByType(moduleType);
        };

        scope.getLocationName = function(location){
          return Locations.getNameById(location.id);
        };

        scope.showDetail = function(deviceId){
          $log.debug("devicesList - Opening detail for device: " + deviceId);
          var newLocation = 'devices.detail';
          if(newLocation === $state.current.name){
            if(($stateParams.deviceId === 'undefined') || ($stateParams.deviceId != deviceId)){
              $log.debug("devicesList - Changing state to: " + newLocation + "(deviceId: " + deviceId + ")");
              $state.go(newLocation, {deviceId: deviceId});
            }else {
              $log.debug("devicesList - Toggling right");
              SidePanel.openRight();
            }
          }else{
            $log.debug("devicesList - Changing state to: " + newLocation + "(deviceId: " + deviceId + ")");
            $state.go(newLocation, {deviceId: deviceId});
          }
        };

        scope.hasLocationDevices = function(locationId){
          return true;
        };
      }
    };
  }]);