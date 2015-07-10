'use strict';

angular.module('beeeOnWebApp')
  .config(['$stateProvider',function ($stateProvider) {
    $stateProvider
      .state('login', {
        url: '/login',
        templateUrl: 'app/routes/login/login.html',
        controller: 'LoginCtrl'
      });
  }]);