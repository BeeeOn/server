'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('overview.description.users', {
        url: '/users',
        views: {
          'right@overview' : {
            templateUrl: 'app/overview/description/users/users.html',
            controller: 'UsersCtrl'
          }
        }
      });
  });