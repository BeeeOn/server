'use strict';

angular.module('beeeOnWebApp')
  .controller('AddGatewayModalCtrl',['$scope','$modalInstance', function ($scope,$modalInstance) {
    $scope.gateId = null;
    $scope.add = function(){
      $modalInstance.close($scope.gateId);
    };
    $scope.cancel = function(){
      $modalInstance.dismiss('cancelled');
    };
  }]);
