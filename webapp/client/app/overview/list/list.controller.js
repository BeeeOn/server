'use strict';

angular.module('beeeOnWebApp')
  .controller('ListCtrl',['$scope','$translate','$timeout','navbar','loadingModal', function ($scope,$translate,$timeout,navbar,loadingModal) {
    $scope.showList = false;
    $scope.addFABisOpen = false;

    $scope.refreshList = function (){
      $translate('OVERVIEW.LIST.LOADING_MODULES').then(function(text){
        loadingModal.show(text + ' (NOT actually)');
      });
      $timeout(function(){
        loadingModal.hide();
      },3000);
    };

    navbar.setLocation('MENU.OVERVIEW');
  }]);
