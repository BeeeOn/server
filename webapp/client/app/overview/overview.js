'use strict';

angular.module('beeeOnWebApp')
  .config(['$stateProvider',function ($stateProvider) {
    $stateProvider
      .state('overview', {
        url: '/overview',
        views: {
          'main' : {
            templateUrl: 'app/overview/overview.html',
            controller: 'OverviewCtrl'
          },
          'content@overview' : {
            templateUrl: 'components/empty/overview/gateway/gateway.html',
            controller: 'EmptyOverviewCtrl'
          }
        },
        params: {
          locationId: 'overview',
          locationName: 'Overview',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      })
    ;
  }]);