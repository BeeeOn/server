'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('modules', {
        url: '/modules/{gatewayId:[0-9]{0,5}}',
        views: {
          'main' : {
            templateUrl: 'app/modules/modules.html',
            controller: 'ModulesCtrl'
          }
        },
        params: {
          locationId: 'modules',
          locationName: 'Modules',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });