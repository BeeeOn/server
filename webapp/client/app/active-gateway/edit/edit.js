'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('active-gateway.edit', {
        url: '/edit',
        views: {
          'right@active-gateway' : {
            templateUrl: 'app/active-gateway/edit/edit.html',
            controller: 'EditCtrl'
          }
        }
      });
  });