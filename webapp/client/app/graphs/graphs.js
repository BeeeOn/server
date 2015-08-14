'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('graphs', {
        url: '/graphs',
        views: {
          'main' : {
            templateUrl: 'app/graphs/graphs.html',
            controller: 'GraphsCtrl'
          }
        },
        params: {
          locationId: 'graphs',
          locationName: 'Graphs',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      })
      .state('graphs.show', {
        url: '/{gatewayId:[0-9]{0,5}}',
        views: {
          'main' : {
            templateUrl: 'app/graphs/graphs.html',
            controller: 'GraphsCtrl'
          }
        },
        params: {
          locationId: 'graphs',
          locationName: 'Graphs',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      })
    ;
  });