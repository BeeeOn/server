'use strict';

angular.module('beeeOnWebApp')
  .controller('DeleteGatewayCtrl',['$scope','$modalInstance','data', function ($scope,$modalInstance,data) {
    $scope.name = data.name;

    $scope.delete = function(){
      $modalInstance.close();
    };
    $scope.cancel = function(){
      $modalInstance.dismiss('cancelled');
    };
  }]);
