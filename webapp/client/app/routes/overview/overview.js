'use strict';

angular.module('beeeOnWebApp')
  .config(['$stateProvider',function ($stateProvider) {
    $stateProvider
      .state('overview', {
        url: '/overview',
        templateUrl: 'app/routes/overview/overview.html',
        controller: 'OverviewCtrl',
        params: {
          locationId: 'overview',
          locationName: 'Overview',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      })
    ;
  }]);