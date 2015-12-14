'use strict';

angular.module('beeeOnWebApp')
  .controller('AddGatewayCtrl', ['$scope','$modalInstance','dialogs', function ($scope,$modalInstance,dialogs) {
    $scope.currentIndex = 0;
    $scope.close = function(){
      $modalInstance.close();
    };
    $scope.goToNext = function(){
      $scope.currentIndex++;
    };
    $scope.skipTutorial = function() {
      $scope.currentIndex = 2;
    };
    $scope.onSwipeLeft = function(){
      if($scope.currentIndex > 0 && $scope.currentIndex < 2){
        $scope.currentIndex++;
      }
    };
    $scope.showAddModal = function(){
      dialogs.create(
        'components/modal/gateway/add/add.html',
        'AddGatewayModalCtrl',
        'lg'
      ).result.then(function(gateId){
          //close the tutorial
          $modalInstance.close();
          //try to add gateway
          console.log("Adding gateway: " + gateId);
        });
    };
  }]);
