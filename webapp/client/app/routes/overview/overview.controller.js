'use strict';

angular.module('beeeOnWebApp')
  .controller('OverviewCtrl', [ '$scope', '$stateParams', '$log','SidePanel' ,function ($scope,$stateParams,$log,SidePanel) {
    $scope.toggleRight= function (){
      SidePanel.toggleRight();
    }
    $scope.gateways = [
      { id:'11111',name: 'Adapter#1',role:'Superuser'},
      { id:'22222',name: 'Adapter#2',role:'Admin'},
      { id:'33333',name: 'Adapter#3',role:'Admin'}
    ];
  }]);
