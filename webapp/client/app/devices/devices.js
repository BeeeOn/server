'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('devices', {
        url: '/devices/{gatewayId:[0-9]{0,5}}',
        views: {
          'main' : {
            templateUrl: 'app/devices/devices.html',
            controller: 'DevicesCtrl'
          }
        },
        params: {
          message: null,
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });
