'use strict';

angular.module('beeeOnWebApp')
  .config(['$stateProvider',function ($stateProvider) {
    $stateProvider
      .state('login', {
        url: '/login',
        views: {
          'main' : {
            templateUrl: 'app/login/login.html',
            controller: 'LoginCtrl'
          }
        }
      });
  }]);