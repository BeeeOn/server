'use strict';

angular.module('beeeOnWebApp')
  .controller('LoginCtrl',['$rootScope','$translate','$scope','$location','$log','$timeout','Modal','Auth','loadingModal',
    function ($rootScope,$translate,$scope,$location,$log,$timeout,Modal,Auth,loadingModal){

    $scope.demoLogin = function(){
      $translate('LOGIN.MODAL_TEXT').then(function(text){
        loadingModal.show(text);
      });
      Auth.login({demo:true})
        .then( function() {
          // Logged in, redirect to home
          $log.debug('LoginCtrl - Redirecting to overview');
          $location.path('/modules');
        })
        .catch( function(err) {
          $scope.errors = err.message;
          $log.error('LoginCtrl - '+err);
        })
        .finally(function() {
          loadingModal.hide();
        });
    };
  }
]);
