'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('watchdog.show', {
        url: '/{gatewayId:[0-9]{0,5}}',
        views: {
          'content' : {
            templateUrl: 'app/watchdog/list/list.html',
            controller: 'WatchdogListCtrl'
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