'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('home', {
        url: '/home',
        views: {
          'main' : {
            templateUrl: 'app/home/home.html',
            controller: 'HomeCtrl'
          },
          'content@home' : {
            templateUrl: 'components/no-gateway/gateway.html',
            controller: 'NoGatewaysCtrl'
          }
        },
        params: {
          message: null,
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });
