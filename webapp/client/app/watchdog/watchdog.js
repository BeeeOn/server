'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('watchdog', {
        url: '/watchdog',
        views: {
          'main' : {
            templateUrl: 'app/watchdog/watchdog.html',
            controller: 'WatchdogCtrl'
          }
        },
        params: {
          locationId: 'watchdog',
          locationName: 'Watchdog',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });