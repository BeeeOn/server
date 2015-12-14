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
          locationId: 'home',
          locationName: 'Home',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });