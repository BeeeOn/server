'use strict';

angular.module('beeeOnWebApp')
  .directive('deviceDetail',[ '$filter','$log','Devices','Modules','Locations','lodash','dialogs','MODULES', function ($filter,$log,Devices,Modules,Locations,lodash,dialogs,MODULES) {
    return {
      templateUrl: 'components/device-detail/device-detail.html',
      restrict: 'E',
      scope : {
        locations : '=',
        device : '='
      },
      link: function (scope, element, attrs) {

        scope.chipModules = [MODULES.BATTERY,MODULES.RSSI,MODULES.REFRESH];
        scope.needTranslationModules = [MODULES.ENUM];
        scope.chips = [];
        scope.groups = ['DEFAULT'];
        scope.ruleHideModules = [];

        var initChips = function(device){
          scope.chips = [];
          //add location chip
          scope.chips.push({
            icon: Locations.getIconById(device.locationid),
            value : Locations.getNameById(device.locationid)
          });
          //add chip modules (for example battery,signal,refresh interval,..)
          angular.forEach(lodash.filter(device.modules,function(module) { return lodash.includes(scope.chipModules,module.model.type)}),function(module){
            scope.chips.push(
              {
                icon: module.model.icon,
                value: $filter('moduleValue')(module,device.type)
              }
            );
          });
        };

        var initGroups = function(device){
          scope.groups = ['DEFAULT'];
          //join default group and all unique groups from device's modules to one array
          scope.groups = lodash.union(scope.groups , lodash.unique(lodash.pluck(device.modules,'model.group')));
          //remove "undefined" groups
          lodash.remove(scope.groups,function(item){ return item === undefined});
          //assign default group to modules, which doesn't have group ( for module filtering in template )
          angular.forEach(device.modules,function(module){
            if(!module.model.group) module.model.group = 'DEFAULT';
          });
        };

        scope.$watch('device', function (newval,oldval) {
          if(newval){
            //assign models to modules
            Modules.initAllModuleModels(scope.device.type,scope.device.modules);
            initChips(scope.device);
            initGroups(scope.device);
            //console.log(scope.device);
            //initialize modules visibility watch
            scope.$watch('device.modules',function(newval,oldval){
              if(newval){
                scope.showModule(scope.device.type,newval);
              }
            },true);
          }
        });

        scope.showModule = function(deviceType,modules){
          scope.ruleHideModules = [];
          angular.forEach(modules,function(module){
            scope.ruleHideModules = lodash.union(scope.ruleHideModules,Modules.getModulesToBeHiddenByRule(deviceType,module));
          });
          angular.forEach(modules,function(module){
            module.isVisible = !(lodash.includes(scope.ruleHideModules, parseInt(module.id)) || lodash.includes(scope.chipModules, module.model.type));
          });
        };

        scope.shouldTranslate = function(deviceType,module){
          var moduleType = Modules.getModuleTypeById(deviceType,module);
          //is module in array of values, that should be translated ?
          return lodash.includes(scope.needTranslationModules,moduleType);
        };

        scope.getName = function(module){
          if(!module.model.name){
            return 'GLOBAL.EMPTY_TRANSLATION';
          }else{
            return 'DEVICES.' + module.model.name;
          }
        };

        scope.actuatorAction = function(module){
          switch (module.model.type){
            case MODULES.ENUM:
              showModal({
                title : "SET_NEW_VALUE",
                translationPrefix : "DEVICES.",
                selected : module.value,
                options : module.model.values
              },module,'radioButton');
              break;
            default:
              if(!module.model.constraints) $log.error("deviceDetail - trying to open \"SET\" modal, but module has no constraints! (moduleId: " + module.id + ",type: " + module.model.type+")");
              showModal(
                {
                  title: 'SET_NEW_VALUE',
                  translationPrefix : "DEVICES.",
                  current : module.value,
                  max: module.model.constraints.max,
                  min: module.model.constraints.min,
                  step: module.model.constraints.granularity,
                  unit: Modules.getModuleUnitByType(module.model.type)
                }
                ,module,'numberPicker');
              break;
          }
        };

        var showModal = function(data,module,modalType) {
          var template = 'components/modal/radio-button/radio-button.html';
          var ctrl = 'RadioButtonModalCtrl';
          switch (modalType){
            case "numberPicker":
              template = 'components/modal/number-picker/number-picker.html';
              ctrl = 'NumberPickerCtrl';
              break;
          }
          dialogs.create(
            template,
            ctrl,
            {
              item : data
            },
            {
              'keyboard': false,
              'backdrop': true,
              'size': 'sm',
              'windowClass': 'radio-button-modal',
              'copy': true
            }
          ).result.then(function(selected){
              switch (modalType){
                case 'numberPicker':
                  changeModuleValue(module,selected);
                  //module.value = selected;
                  break;
                case 'radioButton':
                  changeModuleValue(module,selected.id);
                  break;
              }
            });
        };

        var changeModuleValue = function(module,newValue){
          if(angular.equals(module.value,newValue)) return;
          Devices.setNewModuleState(scope.device.id,module,newValue)
            .then(function(){
            $log.debug("deviceDetail - Module state successfully changed");
            })
            .catch(function(err){
              $log.error(err.data);
            });
        }
      }
    };
  }]);
