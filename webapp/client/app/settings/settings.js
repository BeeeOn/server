'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('settings', {
        url: '/settings',
        views: {
          'main' : {
            templateUrl: 'app/settings/settings.html',
            controller: 'SettingsCtrl'
          }
        },
        params: {
          message: null,
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });
