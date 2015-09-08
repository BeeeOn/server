'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('settings.geolocation', {
        url: '/geolocation',
        views: {
          'right@settings' : {
            templateUrl: 'app/settings/geolocation/geolocation.html',
            controller: 'GeolocationCtrl'
          }
        },
        authenticate: true
      });
  });