'use strict';

angular.module('beeeOnWebApp')
  .controller('RegisterAccountCtrl',[ '$scope','$modalInstance',function ($scope,$modalInstance) {
    $scope.cancel = function(){
      $modalInstance.dismiss('canceled');
    };
    $scope.ok = function(){
      $modalInstance.close("ok");
    };
  }]);
