'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('settings.units', {
        url: '/units',
        templateUrl: 'app/routes/settings/units/units.html',
        controller: 'UnitsCtrl',
        params: {
          locationId: 'settings',
          locationName: 'Settings',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });