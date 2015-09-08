'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('overview.description.edit', {
        url: '/edit',
        views: {
          'right@overview' : {
            templateUrl: 'app/overview/description/edit/edit.html',
            controller: 'EditCtrl'
          }
        }
      });
  });