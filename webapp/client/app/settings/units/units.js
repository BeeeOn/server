'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('settings.units', {
        url: '/units',
        views: {
          'right@settings' : {
            templateUrl: 'app/settings/units/units.html',
            controller: 'UnitsCtrl'
          }
        },
        authenticate: true
      });
  });