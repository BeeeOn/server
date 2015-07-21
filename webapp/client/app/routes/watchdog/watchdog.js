'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('watchdog', {
        url: '/watchdog',
        templateUrl: 'app/routes/watchdog/watchdog.html',
        controller: 'WatchdogCtrl',
        params: {
          locationId: 'watchdog',
          locationName: 'Watchdog',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      })
      .state('watchdog.show', {
        url: '/{gatewayId:[0-9]{0,5}}',
        templateUrl: 'app/routes/watchdog/watchdog.html',
        controller: 'WatchdogCtrl',
        params: {
          locationId: 'watchdog',
          locationName: 'Watchdog',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      })
    ;
  });