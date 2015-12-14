'use strict';

angular.module('beeeOnWebApp')
  .controller('UnitsCtrl',['$scope','$log','$translate','Modules','SidePanel','Settings','dialogs', function ($scope,$log,$translate,Modules,SidePanel,Settings,dialogs) {
    //right panel options
    $scope.rightPanelOptions = SidePanel.getRightPanelOptions();
    $translate('SETTINGS.UNITS.TITLE').then(function(units){
      $scope.rightPanelOptions.location = units;
    });
    //units list
    $scope.units = Settings.getUnits();
    angular.forEach($scope.units,function(value,key){
      value.options = Modules.getUnitOptions(key);
      value.selected = Modules.getSelectedUnit(key);
    });
    //modals
    $scope.showModal = function(unit){
      $log.debug('UnitsCtrl - Showing modal for: ' + unit.title);
      dialogs.create(
        'components/modal/radio-button/radio-button.html',
        'RadioButtonModalCtrl',
        {
          item : unit
        },
        {
          'keyboard': false,
          'backdrop': true,
          'size': 'sm',
          'windowClass': 'radio-button-modal',
          'copy': true
        }
      ).result.then(function(selected){
          unit.selected = selected.id;
          Modules.setSelectedUnit(unit.id,selected.id);
          $log.debug('UnitsCtrl - user selected \"' + unit.options[selected.id].name + '\" as ' + unit.title + ' unit.');
        });
    };

    $scope.filterVisibleUnits = function(items) {
      var result = {};
      angular.forEach(items, function(value, key) {
        if (value.hasOwnProperty('visible') && value.visible) {
          result[key] = value;
        }
      });
      return result;
    };

    SidePanel.toggleRight();
  }]);
