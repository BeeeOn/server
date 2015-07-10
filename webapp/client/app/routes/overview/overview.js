'use strict';

angular.module('beeeOnWebApp')
  .config(['$stateProvider',function ($stateProvider) {
    $stateProvider
      .state('overview', {
        url: '/overview',
        templateUrl: 'app/routes/overview/overview.html',
        controller: 'OverviewCtrl',
        params: {
          locationName: 'Overview',
          locationButtonType: 'menu_toggle' //menu or location. see location-button directive
        },
        authenticate: true
      });
  }]);