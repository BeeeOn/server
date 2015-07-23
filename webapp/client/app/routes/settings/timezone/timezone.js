'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('settings.timezone', {
        url: '/timezone',
        templateUrl: 'app/routes/settings/timezone/timezone.html',
        controller: 'TimezoneCtrl',
        params: {
          locationId: 'settings',
          locationName: 'Settings',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });