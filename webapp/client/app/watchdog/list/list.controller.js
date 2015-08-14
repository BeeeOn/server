'use strict';

angular.module('beeeOnWebApp')
  .controller('WatchdogListCtrl',[ '$scope','$translate','$state','$stateParams','Settings','navbar', function ($scope,$translate,$state,$stateParams,Settings,navbar) {
    $scope.showRules = true;

    $scope.showRightPanel = function(option){
      $state.go('watchdog.show.' + option,{},{location:false});
    };

    $scope.showList = function(option){
      $scope.showRules = option;
    };

    navbar.setLocation('MENU.WATCHDOG');
  }]);
