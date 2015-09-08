'use strict';

angular.module('beeeOnWebApp')
  .config(function ($stateProvider) {
    $stateProvider
      .state('notifications', {
        url: '/notifications',
        views: {
          'main' : {
            templateUrl: 'app/notifications/notifications.html',
            controller: 'NotificationsCtrl'
          }
        },
        params: {
          locationName: 'Notifications',
          locationButtonType: 'location' //menu or location. see location-button directive
        },
        authenticate: true
      });
  });