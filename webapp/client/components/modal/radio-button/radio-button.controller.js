'use strict';

angular.module('beeeOnWebApp')
  .controller('RadioButtonModalCtrl',['$scope','$modalInstance','data', function ($scope,$modalInstance,data) {
    $scope.unit = data.item;
    $scope.title = $scope.unit.translationPrefix ? $scope.unit.translationPrefix + $scope.unit.title : $scope.unit.title;
    $scope.cancel = function(){
      $modalInstance.dismiss('canceled');
    };
    $scope.save = function(selectedOption){
      $modalInstance.close(selectedOption);
    };
  }]);
