'use strict';

angular.module('beeeOnWebApp')
  .controller('DescriptionCtrl',[ '$scope','$state','$stateParams','$log' ,'Gateway', function ($scope,$state,$stateParams,$log,Gateway) {
    //retrieve gateway info
    Gateway.getSelectedInfo($stateParams.gatewayId).then(function(data){
      $scope.gatewayInfo = data;
    },function(err){
      //TODO error handling
      $log.error(err.data);
    });

    $scope.showDevices = function(){
      $state.go('overview.show',{gatewayId:$scope.gatewayInfo.aid});
    };

    $scope.showUsers = function(){

    };
  }]);
