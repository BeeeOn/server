'use strict';

angular.module('beeeOnWebApp')
  .controller('NumberPickerCtrl',['$scope','$modalInstance','data', function ($scope,$modalInstance,data) {
    $scope.item = data.item;
    $scope.cancel = function(){
      $modalInstance.dismiss('canceled');
    };
    $scope.pickerValue = $scope.item.current;
    $scope.save = function(selectedOption){
      $modalInstance.close(selectedOption);
    };
  }]);
