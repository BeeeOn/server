'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('watchdog.show.add', {
        url: '/add',
        views: {
          'right@watchdog' : {
            templateUrl: 'app/watchdog/add/add.html',
            controller: 'AddRuleCtrl'
          }
        }
      });
  });