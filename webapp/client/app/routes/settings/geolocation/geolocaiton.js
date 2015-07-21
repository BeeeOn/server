'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('settings.geolocation', {
        url: '/geolocation',
        templateUrl: 'app/routes/settings/geolocation/geolocation.html',
        controller: 'GeolocationCtrl',
        params: {
          locationId: 'settings',
          locationName: 'Settings',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });