'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
    .state('active-gateway', {
      url: '/active-gateway/{gatewayId:[0-9]{0,5}}',
      views: {
        'main' : {
          templateUrl: 'app/active-gateway/active-gateway.html',
          controller: 'ActiveGatewayCtrl'
        }
      },
      params: {
        message: null,
        reloadData: false,
        locationButtonType: 'panel_toggle_left' //see location-button directive
      },
      authenticate: true
    });
  });
