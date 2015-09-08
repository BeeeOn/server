'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('settings.timezone', {
        url: '/timezone',
        views: {
          'right@settings' : {
            templateUrl: 'app/settings/timezone/timezone.html',
            controller: 'TimezoneCtrl'
          }
        },
        authenticate: true
      });
  });