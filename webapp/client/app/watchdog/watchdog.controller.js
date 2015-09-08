'use strict';

angular.module('beeeOnWebApp')
  .controller('WatchdogCtrl',['$scope','$translate','SidePanel','navbar', function ($scope,$translate,SidePanel,navbar) {
    $scope.rightPanelOptions = SidePanel.getRightPanelOptions();
    navbar.setLocation('MENU.WATCHDOG');
  }]);
