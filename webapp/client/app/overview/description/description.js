'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('overview.description', {
        url: '/{gatewayId:[0-9]{0,5}}/description',
        views: {
          'content' : {
            templateUrl: 'app/overview/description/description.html',
            controller: 'DescriptionCtrl'
          }
        },
        params: {
          locationId: 'overview',
          locationName: 'Overview',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });