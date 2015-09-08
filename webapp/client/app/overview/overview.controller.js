'use strict';

angular.module('beeeOnWebApp')
  .controller('OverviewCtrl', [ '$scope', '$stateParams', '$log','$translate','navbar','SidePanel' ,function ($scope,$stateParams,$log,$translate,navbar,SidePanel) {
    $scope.toggleRight= function (){
      SidePanel.toggleRight();
    };

    $scope.rightPanelOptions = SidePanel.getRightPanelOptions();

    navbar.setLocation('MENU.OVERVIEW');
  }]);
