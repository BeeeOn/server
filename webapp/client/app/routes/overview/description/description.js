'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('overview.description', {
        url: '/{gatewayId:[0-9]{0,5}}/description',
        templateUrl: 'app/routes/overview/description/description.html',
        controller: 'DescriptionCtrl',
        params: {
          locationId: 'overview',
          locationName: 'Settings for',
          locationButtonType: 'panel_toggle_left' //see location-button directive
        },
        authenticate: true
      });
  });