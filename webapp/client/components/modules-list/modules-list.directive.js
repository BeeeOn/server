'use strict';

angular.module('beeeOnWebApp')
  .directive('modulesList',['$state','$stateParams','$log','$translate','lodash','SidePanel','Devices','Modules','Locations','MODULES', function ($state,$stateParams,$log,$translate,lodash,SidePanel,Devices,Modules,Locations,MODULES) {
    return {
      templateUrl: 'components/modules-list/modules-list.html',
      restrict: 'E',
      scope : {
        locations : '=',
        devices : '='
      },
      link: function (scope, element, attrs) {
        scope.notVisibleModules = [MODULES.BATTERY,MODULES.RSSI,MODULES.REFRESH];
        scope.needTranslationModules = [MODULES.ENUM];
        scope.ruleHideModules = [];

        scope.showModule = function(deviceType,modules){
          scope.ruleHideModules = [];
          angular.forEach(modules,function(module){
            scope.ruleHideModules = lodash.union(scope.ruleHideModules,Modules.getModulesToBeHiddenByRule(deviceType,module));
          });
          angular.forEach(modules,function(module){
            module.isVisible = !(lodash.includes(scope.ruleHideModules, parseInt(module.id)) || lodash.includes(scope.notVisibleModules, module.model.type));
          });
        };
        //initialize devices
        scope.$watch('devices', function (newval,oldval) {
          if(newval){
            angular.forEach(scope.devices,function(device,pos){
              //assign models to modules
              Modules.initAllModuleModels(device.type,device.modules);
              scope.$watch('devices['+pos+'].modules',function(newmodules,oldmodule){
                if(newmodules){
                  scope.showModule(device.type,newmodules);
                }
              },true);
            });
          }
        });

        scope.shouldTranslate = function(module){
          //is module in array of values, that should be translated ?
          return lodash.includes(scope.needTranslationModules,module.model.type);
        };

        scope.getIcon = function(module){
          return module.model.icon;
        };

        scope.getName = function(module){
          if(!module.model.name){
            return 'GLOBAL.EMPTY_TRANSLATION';
          }else{
            return 'DEVICES.' + module.model.name;
          }
        };

        scope.getLocationName = function(location){
          return Locations.getNameById(location.id);
        };

        scope.getGroupName = function(module){
          if(module.model && module.model.group){
            return 'DEVICES.'+module.model.group;
          }else{
            return 'GLOBAL.EMPTY_TRANSLATION';
          }
        };

        scope.showDetail = function(deviceId){
          $log.debug("modulesList - Opening detail for device: " + deviceId);
          var newLocation = 'modules.detail';
          if(newLocation.split('.')[0] === $state.current.name.split('.')[0]){
            if(($stateParams.deviceId === 'undefined') || ($stateParams.deviceId != deviceId)){
              $log.debug("modulesList - Changing state to: " + newLocation + "(deviceId: " + deviceId + ")");
              $state.go(newLocation, {deviceId: deviceId});
            }else {
              console.log("modulesList - Toggling right");
              SidePanel.toggleRight();
            }
          }else{
            $log.debug("modulesList - Changing state to: " + newLocation + "(deviceId: " + deviceId + ")");
            $state.go(newLocation, {deviceId: deviceId});
          }
        };
      }
    };
  }]);