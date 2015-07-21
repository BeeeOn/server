'use strict';

angular.module('beeeOnWebApp')
  .controller('LoginCtrl',['$rootScope','$scope','$location','$log','$timeout','Modal','Auth',
    function ($rootScope,$scope,$location,$log,$timeout,Modal,Auth){

    $scope.demoLogin = function(){
      Auth.login({demo:true})
        .then( function() {
          // Logged in, redirect to home
          $log.debug('Redirecting to overview');
          $location.path('/overview');
        })
        .catch( function(err) {
          $scope.errors = err.message;
          $log.error(err);
        });
    };
  }
]);
