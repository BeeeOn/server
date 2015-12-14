'use strict';

angular.module('beeeOnWebApp')
  .controller('WatchdogCtrl',['$scope','$translate','$state','SidePanel','navbar', function ($scope,$translate,$state,SidePanel,navbar) {
    $scope.rightPanelOptions = SidePanel.getRightPanelOptions();
    $scope.showRules = true;

    $scope.showRightPanel = function(option){
      $state.go('watchdog.' + option,{},{location:false});
    };

    $scope.showList = function(option){
      $scope.showRules = option;
    };
    navbar.setLocation('MENU.WATCHDOG');
  }]);
