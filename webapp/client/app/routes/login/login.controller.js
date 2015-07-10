'use strict';

angular.module('beeeOnWebApp')
  .controller('LoginCtrl',['$rootScope','$scope','$location','$log','$timeout','Modal','Auth',
    function ($rootScope,$scope,$location,$log,$timeout,Modal,Auth){

    $scope.demoLogin = function(){
      Auth.demoLogin();
      if(Auth.getCurrentUser){
        $location.path('/overview');
      };
    }

    /*$scope.showAlert = function(){
      Modal.information.progress("Logging in")();
    };


    $log.debug($scope.showAlert);

    $scope.changeProgress = function(){
      $rootScope.$broadcast('CHANGE_MESSAGE','asdasdasdasdasdasd');
    };

    $timeout(function(){
      $scope.changeProgress();
    },3000);*/
  }
]);
