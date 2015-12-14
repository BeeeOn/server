'use strict';

angular.module('beeeOnWebApp')
  .filter('moduleValue',['$translate','Modules','MODULES', function ($translate,Modules,MODULES) {
    return function (module,deviceType) {
      switch (Modules.getModuleTypeById(deviceType,module)){
        case MODULES.ENUM:
          return 'DEVICES.' + Modules.getModuleEnumValueName(deviceType,module);
        default :
          return Modules.getConvertedValue(deviceType,module)+ " " + Modules.getModuleUnit(deviceType,module);
      }
    };
  }]);
