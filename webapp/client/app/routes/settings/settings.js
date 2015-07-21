'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('settings', {
        url: '/settings',
        templateUrl: 'app/routes/settings/settings.html',
        controller: 'SettingsCtrl',
        params: {
          isEmpty: true,
          locationId: 'settings',
          locationName: 'Settings',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });