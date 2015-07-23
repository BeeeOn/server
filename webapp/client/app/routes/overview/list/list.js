'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('overview.show', {
        url: '/{gatewayId:[0-9]{0,5}}',
        templateUrl: 'app/routes/overview/list/list.html',
        controller: 'ListCtrl',
        params: {
          locationId: 'overview',
          locationName: 'Overview',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });