'use strict';

angular.module('beeeOnWebApp')
  .controller('HomeCtrl',[ '$scope', '$stateParams', '$log','$translate','navbar','SidePanel' ,function ($scope,$stateParams,$log,$translate,navbar,SidePanel) {
    $scope.toggleRight= function (){
      SidePanel.toggleRight();
    };
    $scope.hideOnScroll = false;
    $scope.rightPanelOptions = SidePanel.getRightPanelOptions();

    navbar.setLocation('GLOBAL.HOME');
  }]);
