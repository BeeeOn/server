'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('active-gateway.users', {
        url: '/users',
        views: {
          'right@active-gateway' : {
            templateUrl: 'app/active-gateway/users/users.html',
            controller: 'UsersCtrl'
          }
        }
      });
  });